#include "MessageNode.h"
#include "config.h"
MessageNode::MessageNode(short message_length) :
    _data(new char[message_length + 1]),
    _total_length(message_length),
    _current_length(0)
{
    memset(_data, 0, _total_length + 1); // Initialize the data buffer
    _data[_total_length] = '\0'; // Ensure null-termination
}

MessageNode::~MessageNode()
{
    delete[] _data; // Ensure to free allocated memory
    _data = nullptr; // Avoid dangling pointer
    _total_length = 0;
    _current_length = 0;
}

void MessageNode::Clear()
{
    _current_length = 0;
    // Note: _data should be allocated before using Clear, otherwise this will lead to undefined behavior
    if (_data == nullptr) {
        _data = new char[_total_length]; // Allocate memory if not already allocated
    }
    memset(_data, 0, _total_length); // Clear the data buffer
}

char *MessageNode::GetData()
{
    return _data;
}

short MessageNode::GetTotalLength() const
{
    return _total_length;
}

short MessageNode::GetCurrentLength() const
{
    return _current_length;
}

void MessageNode::SetCurrentLength(short length)
{
    if (length < 0 || length > _total_length) {
        throw std::out_of_range("Length out of bounds");
    }
    _current_length = length;
}

RecieveMessageNode::RecieveMessageNode(short message_length, short message_type) :
    MessageNode(message_length),
    _message_type(message_type)
{
}

short RecieveMessageNode::GetMessageType() const
{
    return _message_type;
}

SendMessageNode::SendMessageNode(const char *message, short message_length, short message_type) :
    MessageNode(message_length + DATA_HEADER_SIZE),
    _message_type(message_type)
{
    // host to net byte order conversion for message length/type
    short net_length = boost::asio::detail::socket_ops::host_to_network_short(message_length);
    short net_type = boost::asio::detail::socket_ops::host_to_network_short(message_type);

    // Copy the message length and type into the data buffer
    memcpy(_data, &net_type, DATA_MESSAGE_TYPE_SIZE);
    memcpy(_data + DATA_MESSAGE_TYPE_SIZE, &net_length, DATA_MESSAGE_LENGTH);
    // Copy the actual message data
    if (message_length > 0 && message != nullptr) {
        memcpy(_data + DATA_HEADER_SIZE, message, message_length);
        // Ensure null-termination
        _data[DATA_HEADER_SIZE + message_length] = '\0';
    }
    _current_length = message_length + DATA_HEADER_SIZE; // Update current length to include header
}

short SendMessageNode::GetMessageType() const {
    return _message_type;
}

LogicNode::LogicNode(std::shared_ptr<CSession> session, std::shared_ptr<RecieveMessageNode> message_node) :
    _session(session),
    _message_node(message_node)
{
}
