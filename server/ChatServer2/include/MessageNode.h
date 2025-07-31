#pragma once
#include <cstring>
#include <boost/asio.hpp>
#include "LogicSystem.h"
#include "CSession.h"

namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class MessageNode {
public:
    MessageNode(short message_length);
    ~MessageNode();
    void Clear();
    char* GetData();
    short GetTotalLength() const;
    short GetCurrentLength() const;
    void SetCurrentLength(short length);
protected:
    char *_data;
    short _total_length;
    short _current_length;
};

/*
    从客户端收到的消息，_data()存储消息内容，_message_type存储消息类型，message_length存储消息长度
    注意：_data长度即为message_length，（首先从CSession::read_head读取客户端发送的消息类型和消息长度, 然后
    利用消息长度从CSession::read_body拿到客户端发送的具体消息内容，接下来就可以把消息内容写入到_data中， 接下来构造
    把RecieveMessageNode构造成LogicNode推送到(生产者)LogicSystem的消息队列message_queue，并发送notyfy_one信号通知消费者消费节点）
*/

class RecieveMessageNode : public MessageNode {
public:
    RecieveMessageNode(short message_length, short message_type);
    ~RecieveMessageNode() = default;
    short GetMessageType() const;
    void SetMessageType(short message_type) { _message_type = message_type; }
private:
    short _message_type;
};

/*
    这是要发送给客户端的消息，_data()存储消息内容，_message_type存储消息类型，message_length存储消息长度
    注意：_data长度即为message_length + DATA_HEADER_SIZE，DATA_HEADER_SIZE是消息头的长度，
    其中前2个字节存储消息长度，后2个字节存储消息类型
    由CSession的send函数直接把TLV格式的消息发送给客户端，客户端会先读取消息头的前4个字节，获取消息长度和消息类型，
    然后再读取消息内容。send函数由LoginSystem在验证好RecieveMessageNode的数据后调用（如验证登陆时的token和uid， 过程是
    利用Grpc和StatusServer进行验证），每个session都维护好一个send_queue来保证异步发送消息的顺序性，
    发送的消息会被LogicSystem封装成SendMessageNode对象，放入到send_queue中，
    然后通过CSession::send函数发送给客户端
*/
class SendMessageNode : public MessageNode {
public:
    SendMessageNode(const char* message, short message_length, short message_type);
    ~SendMessageNode() = default;
    short GetMessageType() const;
    void SetMessageType(short message_type) { _message_type = message_type; }
private:
    short _message_type;
};

class LogicNode {
public:
    LogicNode(std::shared_ptr<CSession> session, std::shared_ptr<RecieveMessageNode> message_node);
    ~LogicNode() = default;
    std::shared_ptr<RecieveMessageNode> GetMessageNode() const { return _message_node; }
    std::shared_ptr<CSession> GetSession() const { return _session; }
private:
    std::shared_ptr<RecieveMessageNode> _message_node;
    std::shared_ptr<CSession> _session;
};
