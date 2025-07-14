#include "CSession.h"
#include "CServer.h"
#include "MessageNode.h"

CSession::CSession(boost::asio::io_context& ioc, std::shared_ptr<CServer> server) :
    _socket(ioc),
    _server(server),
    _uuid(boost::uuids::to_string(boost::uuids::random_generator()())),
    _send_queue{},
    _send_mutex{},
    _is_sending(false),
    _is_stopped(false)
{

}

CSession::~CSession() {
    Close();
    spdlog::info("Session with UUID {} has been destroyed.", _uuid);
}

void CSession::Start() {
    ReadHead(DATA_HEADER_SIZE);
}

tcp::socket& CSession::GetSocket() {
    return _socket;
}

std::string CSession::GetUUID() const {
    return _uuid;
}

void CSession::ReadHead(short head_length)
{
    try
    {
        std::memset(_header, 0, sizeof(_header));
        auto self = shared_from_this();
        net::async_read(_socket, net::buffer(_header, head_length), [self, head_length](const boost::system::error_code &ec, std::size_t bytes_transferred)
                        {
        if (ec) {
            spdlog::error("Failed to read head: {}", ec.message());
            self->Close();
            return;
        }
        if (bytes_transferred < head_length) {
            spdlog::warn("Incomplete head read, expected {}, got {}", head_length, bytes_transferred);
            self->Close();
            return;
        }
        // Process the head data, extract message length and type)(net to host byte order if necessary, big endian)
        short message_length, message_type;
        std::memcpy(&message_type, self->_header, sizeof(short)); // TLV format: first 2 bytes for message type
        std::memcpy(&message_length, self->_header + sizeof(short), sizeof(short)); // Next 2 bytes for message length
        message_length = net::detail::socket_ops::network_to_host_short(message_length);
        message_type = net::detail::socket_ops::network_to_host_short(message_type); // 1005
        if (message_length > MAX_MESSAGE_LENGTH - DATA_HEADER_SIZE) {
            spdlog::error("Message length {} exceeds maximum allowed size.", message_length);
            self->Close();
            return;
        }
        self->ReadBody(message_length, message_type);
        });
    }
    catch (const std::exception &e)
    {
        spdlog::error("Exception in ReadHead: {}", e.what());
        Close();
        return;
    }
}

void CSession::ReadBody(short message_length, short message_type) {
    try {
        auto self = shared_from_this();
        self->_body.resize(message_length);
        net::async_read(_socket, net::buffer(self->_body.data(), message_length), [self, message_length, message_type]
                        (const boost::system::error_code& ec, std::size_t bytes_transferred) {
            if (ec) {
                spdlog::error("Failed to read body: {}", ec.message());
                self->Close();
                return;
            }

            if (bytes_transferred < message_length) {
                spdlog::warn("Incomplete body read, expected {}, got {}", message_length, bytes_transferred);
                self->Close();
                return;
            }
            // Log the received message
            std::string message(self->_body.begin(), self->_body.end());
            spdlog::info("[CSession]Received message: {}", message);
            // Process the body data to a LogicNode and submit to the LogicSystem
            std::shared_ptr<RecieveMessageNode> message_node = std::make_shared<RecieveMessageNode>(message_length, message_type);
            std::memcpy(message_node->GetData(), self->_body.data(), message_length);
            message_node->SetCurrentLength(message_length);
            message_node->SetMessageType(message_type);
            std::shared_ptr<LogicNode> logic_node = std::make_shared<LogicNode>(self, message_node);

            // Post the logic node to the LogicSystem for processing
            LogicSystem::GetInstance()->PostLogicNode(self, message_node);
            self->ReadHead(); // Continue reading the next message head
        });
    } catch (const std::exception &e) {
        spdlog::error("Exception in ReadBody: {}", e.what());
        Close();
        return;
    }
}

void CSession::Send(const char* data, short length, short message_type) {
    if (_is_stopped) {
        spdlog::warn("Session {} is stopped, cannot send data.", _uuid);
        return;
    }

    auto queue_size = _send_queue.size();
    if (queue_size >= MAX_SEND_QUEUE) {
        spdlog::warn("Send queue is full ({}), dropping message.", queue_size);
        return;
    }
    // Create a SendMessageNode and add it to the send queue
    auto new_message_node = std::make_shared<SendMessageNode>(data, length, message_type);
    {
        std::lock_guard<std::mutex> lock(_send_mutex);
        _send_queue.push(new_message_node);
    } // release the lock

    if (_is_sending.load()) {
        spdlog::warn("Already sending, message added to queue.");
        return; // Already sending, just add to the queue
    }
    // If not already sending, start sending the message
    auto send_message = _send_queue.front();
    _is_sending = true; // Set sending flag

    auto self = shared_from_this();
    net::async_write(_socket, net::buffer(send_message->GetData(), send_message->GetCurrentLength()), [self, send_message]
    (const boost::system::error_code& ec, std::size_t bytes_transferred) {
        self->HandleWrite(ec, bytes_transferred, send_message, self);
    });

}

void CSession::Send(std::string message, short message_type)
{
    Send(message.data(), message.size(), message_type);
}

void CSession::Close()
{
    try
    {
        if (_socket.is_open())
        {
            boost::system::error_code ec;
            _socket.shutdown(tcp::socket::shutdown_both, ec);
            if (ec)
            {
                spdlog::error("Failed to shutdown socket: {}", ec.message());
            }
            _socket.close(ec);
            if (ec)
            {
                spdlog::error("Failed to close socket: {}", ec.message());
            }
        }
        _is_stopped = true;
        _is_sending = false;
        _send_queue = std::queue<std::shared_ptr<SendMessageNode>>(); // Clear the send queue
        // Remove from server
        if (_server)
        {
            try {
                _server->ClearSession(_uuid);
            } catch (const std::exception& e) {
                spdlog::warn("Failed to clear session from server: {}", e.what());
            }
        }
    }
    catch (const std::exception &e)
    {
        spdlog::error("Exception in Close: {}", e.what());
    }
}

void CSession::HandleWrite(const boost::system::error_code &ec, std::size_t bytes_transferred, std::shared_ptr<SendMessageNode> send_message, std::shared_ptr<CSession> self)
{
    try
    {
        if (ec)
        {
            spdlog::error("Failed to send message: {}", ec.message());
            self->Close();
            return;
        }

        if (bytes_transferred < send_message->GetCurrentLength())
        {
            spdlog::warn("Incomplete message sent, expected {}, got {}", send_message->GetCurrentLength(), bytes_transferred);
            self->Close();
            return;
        }

        std::lock_guard<std::mutex> lock(self->_send_mutex);
        self->_send_queue.pop(); // Remove the message from the queue
        spdlog::info("[CSession]Message sent successfully, bytes transferred: {}", bytes_transferred);
        if (_is_sending || self->_send_queue.empty())
        {
            self->_is_sending = false; // No more messages to send, reset sending flag
            spdlog::info("[CSession]All messages sent, send queue is now empty.");
            return;
        }
        // Start sending the next message in the queue
        auto next_message = self->_send_queue.front();
        self->_is_sending = true; // Set sending flag
        net::async_write(self->_socket, net::buffer(next_message->GetData(), next_message->GetCurrentLength()),
                        [self, next_message](const boost::system::error_code &ec, std::size_t bytes_transferred)
                        {
                        self->HandleWrite(ec, bytes_transferred, next_message, self);
                        });
    }
    catch (const std::exception &e)
    {
        spdlog::error("Exception in HandleWrite: {}", e.what());
    }
}
