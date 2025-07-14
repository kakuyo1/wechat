#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <map>
#include <boost/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "config.h"
#include <queue>
#include <mutex>
#include <atomic>
#include <spdlog/spdlog.h>
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
/*
    CSession类代表一个客户端连接的会话，负责处理与客户端的通信
    包括读取消息头和消息体，发送消息等操作
    每个CSession对象都对应一个唯一的UUID，用于标识该会话
    CSession对象由CServer管理，CServer负责接受新的连接并创建CSession对象

    CSession利用char _data[MAX_MESSAGE_LENGTH];作为接收数据的缓冲区，负责存储从客户端接收到的消息数据
    read_head函数读取消息头，获取消息长度和消息类型
    read_body函数读取消息体，处理具体的消息内容
    读取完一次消息头和体后，CSession会将消息封装成RecieveMessageNode对象再加工为LogicNode，并推送到LogicSystem的消息队列中
    由LogicSystem的消费者线程处理消息队列中的LogicNode(比如验证客户端登陆请求的uid和token是否匹配)

    注意：LogicSystem不进行消息的发送和接受，只负责处理消息，最终还是会调用CSession的send函数将处理结果发送给客户端
*/

namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
class LogicNode;
class RecieveMessageNode;
class SendMessageNode;
class CServer;
class CSession : public std::enable_shared_from_this<CSession> {
public:
    CSession(boost::asio::io_context& ioc, std::shared_ptr<CServer> server);
    ~CSession();
    void Start();
    tcp::socket& GetSocket();
    std::string GetUUID() const;
    void Send(const char* data, short length, short message_type);
    void Send(std::string message, short message_type);
private:
    void ReadHead(short head_length = DATA_HEADER_SIZE);
    void ReadBody(short message_length, short message_type);
    void Close();
    void HandleWrite(const boost::system::error_code& ec, std::size_t bytes_transferred, std::shared_ptr<SendMessageNode> send_message, std::shared_ptr<CSession> self);
private:
    tcp::socket _socket;
    std::string _uuid;
    std::shared_ptr<CServer> _server;
    std::string uuid;
    std::queue<std::shared_ptr<SendMessageNode>> _send_queue;
    // std::shared_ptr<RecieveMessageNode> _receive_message_body;
    // std::shared_ptr<MessageNode> _receive_message_head;
    std::mutex _send_mutex;
    std::atomic<bool> _is_sending{false};
    std::atomic<bool> _is_stopped{false};
    char _header[DATA_HEADER_SIZE];
    std::vector<char> _body;
};