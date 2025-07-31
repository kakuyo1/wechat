#include "tcpmanager.h"
#include "usermanager.h"
#include <QJsonArray>

TcpManager::TcpManager() :
    _socket(new QTcpSocket(this)),
    _host(""),
    _port(0),
    _messageLength(0),
    _messageType(0),
    _hasIncompleteBody(false)
{
    // connect the signal connected
    connect(_socket, &QTcpSocket::connected, this, [this](){
        qDebug() << "Connected to chat server at" << _host << ":" << _port;
        emit signal_connect_to_chatserver_success(true); // just connected , need authentication
    });

    // connect the signal disconnected
    connect(_socket, &QTcpSocket::disconnected, this, [](){
        qDebug() << "Disconnected from chat server";
    });

    // connect the signal errorOccured
    connect(_socket, &QTcpSocket::errorOccurred, this, [this](QTcpSocket::SocketError socketError){
        qDebug() << "Socket error occurred:" << socketError;
        switch(socketError) {
            case QTcpSocket::RemoteHostClosedError:
                qDebug() << "Remote host closed the connection.";
                break;
            case QTcpSocket::HostNotFoundError:
                qDebug() << "Host not found.";
                emit signal_connect_to_chatserver_success(false);
                break;
            case QTcpSocket::ConnectionRefusedError:
                qDebug() << "Connection refused by the server.";
                emit signal_connect_to_chatserver_success(false);
                break;
            case QTcpSocket::NetworkError:
                qDebug() << "Network error occurred.";
                emit signal_connect_to_chatserver_success(false);
                break;
            case QTcpSocket::SocketTimeoutError:
                qDebug() << "Socket operation timed out.";
                emit signal_connect_to_chatserver_success(false);
                break;
            default:
                qDebug() << "Other socket error occurred:" << socketError;
                emit signal_connect_to_chatserver_success(false);
        }
    });

    // connect the signal send_data to the slot
    connect(this, &TcpManager::signal_send_data, this, &TcpManager::slot_send_data);

    // connect the signal readyRead to the slot
    connect(_socket, &QTcpSocket::readyRead, this, [this](){
        _messageBuffer.append(_socket->readAll());
        QDataStream stream(&_messageBuffer, QIODevice::ReadOnly);
        stream.setByteOrder(QDataStream::BigEndian);
        stream.setVersion(QDataStream::Qt_6_9);
        while(true) {
            // read the header
            if (!_hasIncompleteBody) {
                if (_messageBuffer.size() < static_cast<long long>(sizeof(quint16) * 2)) {
                    return; // not enough data for header
                }
                // enough for header
                stream >> _messageType >> _messageLength;
                _messageBuffer.remove(0, sizeof(quint16) * 2); // remove header from buffer
            }
            // check if we have enough data for the body
            if (_messageBuffer.size() < static_cast<long long>(_messageLength)) {
                _hasIncompleteBody = true; // not enough data for body
                return;
            }
            // we have enough data for the body
            QByteArray bodyData = _messageBuffer.left(_messageLength);// read _messageLength bytes from left(0)
            _messageBuffer.remove(0, _messageLength); // remove body from buffer
            _hasIncompleteBody = false; // reset incomplete body flag
            // handle the message
            handleMessage(static_cast<RequestType>(_messageType), _messageLength, bodyData);
        }
    });
    // initialize the handlers
    initHandlers();
}

void TcpManager::initHandlers()
{
    // handler for login authentication
    _handlers[RequestType::TYPE_LOGIN_CHAT_SERVER_RESPONSE] = [this](RequestType type, int len, QByteArray data) {
        Q_UNUSED(len);
        Q_UNUSED(type);
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull() || doc.isEmpty()) {
            qDebug() << "Invalid JSON response from chat server";
            emit signal_login_failed();
            return;
        }
        if (!doc.isObject()) {
            qDebug() << "Response is not a JSON object";
            emit signal_login_failed();
            return;
        }
        QJsonObject jsonObj = doc.object();
        if (!jsonObj.contains("error")) {
            qDebug() << "Response does not contain 'error' field";
            emit signal_login_failed();
            return;
        }
        int errorCode = jsonObj.value("error").toInt();
        if (errorCode != static_cast<int>(ErrorCode::SUCCESS)) {
            qDebug() << "Login authentication failed with error code:" << errorCode;
            if (errorCode == static_cast<int>(ErrorCode::ERROR_INVALID_AUTH_PARAMETERS)) {
                qDebug() << "Invalid authentication parameters";
                emit signal_login_failed();
            } else if (errorCode == static_cast<int>(ErrorCode::ERROR_UID_NOT_FOUND)) {
                qDebug() << "UID not found in the chat server";
                emit signal_login_failed();
            } else if (errorCode == static_cast<int>(ErrorCode::ERROR_TOKEN_MISMATCH)) {
                qDebug() << "Token mismatch error";
                emit signal_login_failed();
            } else if (errorCode == static_cast<int>(ErrorCode::ERROR_USER_ALREADY_ONLINE)) {
                qDebug() << "User is already online";
                emit signal_login_failed_online_already();
            } else {
                qDebug() << "Authentication failed with error code:" << errorCode;
            }
            return;
        }
        // Authentication successful
        qDebug() << "Login authentication successful";
        qDebug() << "Response message: " << jsonObj.value("message").toString();
        qDebug() << "Response error: " << errorCode;
        qDebug() << "Response uid: " << jsonObj.value("uid").toInt();
        qDebug() << "Response token: " << jsonObj.value("token").toString();
        qDebug() << "Response gender:" << jsonObj.value("gender").toInt();
        qDebug() << "Response name:" << jsonObj.value("name").toString();
        qDebug() << "Response nickname:" << jsonObj.value("nickname").toString();
        qDebug() << "Response email:" << jsonObj.value("email").toString();
        qDebug() << "Response icon:" << jsonObj.value("icon").toString();
        qDebug() << "Response desc:" << jsonObj.value("desc").toString();
        qDebug() << "Response password:" << jsonObj.value("password").toString();

        // 将信息录入UserManager
        UserManager::GetInstance()->setUid(jsonObj.value("uid").toInt());
        UserManager::GetInstance()->setName(jsonObj.value("name").toString());
        UserManager::GetInstance()->setIconPath(jsonObj.value("icon").toString());
        UserManager::GetInstance()->setDescription(jsonObj.value("desc").toString());
        UserManager::GetInstance()->setEmail(jsonObj.value("email").toString());
        UserManager::GetInstance()->setGender(jsonObj.value("gender").toInt());
        //  friendlistRequest
        if (!jsonObj.contains("friend_request_list") || !jsonObj["friend_request_list"].isArray()) {
            qDebug() << "Response does not contain 'friend_request_list' or it is not an array";
            emit signal_login_failed();
            return;
        }

        if (jsonObj["friend_request_list"].toArray().isEmpty()) {
            qDebug() << "No friend requests found in the response";
        } else {
            QJsonArray friendRequestArray = jsonObj["friend_request_list"].toArray();
            std::vector<std::shared_ptr<FriendListItemInfo>> friendRequestList;

            for (const QJsonValue &value : friendRequestArray) {
                if (!value.isObject()) {
                    qDebug() << "Friend request item is not an object";
                    continue;
                }
                QJsonObject itemObj = value.toObject();
                if (!itemObj.contains("from_uid") || !itemObj.contains("status") || !itemObj.contains("from_desc")
                    || !itemObj.contains("from_icon") || !itemObj.contains("from_name")) {
                    qDebug() << "Missing field from friend request item";
                    continue;
                }
                int from_uid = itemObj["from_uid"].toInt();
                int status = itemObj["status"].toInt();
                QString from_name = itemObj.value("from_name").toString();
                QString from_desc = itemObj.value("from_desc").toString();
                QString from_avatarPath = itemObj.value("from_icon").toString();
                int self_uid = UserManager::GetInstance()->getUid();
                qDebug() << "Friend request from_uid:" << from_uid;
                FriendListItemInfo requestInfo(self_uid, from_uid, from_name, from_desc, from_avatarPath, status);
                auto requestInfoptr = std::make_shared<FriendListItemInfo>(requestInfo);
                friendRequestList.push_back(requestInfoptr);
            }
            UserManager::GetInstance()->intialFriendRequestListAfterLogin(friendRequestList); // 初始化好友申请列表
        }

        // contactlist
        emit signal_switchto_chatdialog();
    };

    // handle for search user response
    _handlers[RequestType::MESSAGE_CHATSERVER_SEARCH_USER_RESPONSE] = [this](RequestType type, int len, QByteArray data) {
        Q_UNUSED(len);
        Q_UNUSED(type);
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull() || doc.isEmpty()) {
            qDebug() << "Invalid JSON response from chat server";
            emit signal_search_user_failed("Invalid JSON response from chat server");
            return;
        }
        if (!doc.isObject()) {
            qDebug() << "Response is not a JSON object";
            emit signal_search_user_failed("Response is not a JSON object");
            return;
        }
        QJsonObject jsonObj = doc.object();
        if (!jsonObj.contains("error") && !jsonObj.contains("message")) {
            qDebug() << "Response does not contain 'error/message' field";
            emit signal_search_user_failed("Response does not contain 'error/message' field");
            return;
        }
        int errorCode = jsonObj.value("error").toInt();
        if (errorCode != static_cast<int>(ErrorCode::SUCCESS)) {
            qDebug() << "Search user failed with error code:" << errorCode;
            emit signal_search_user_failed(jsonObj.value("message").toString());
            return;
        }
        // Create a SearchInfo object from the response when Search user successful
        auto uid = jsonObj.value("uid").toInt();
        auto name = jsonObj.value("name").toString();
        auto nickname = jsonObj.value("nickname").toString();
        auto desc = jsonObj.value("desc").toString();
        auto gender = jsonObj.value("gender").toInt();
        auto icon = jsonObj.value("icon").toString();
        auto email = jsonObj.value("email").toString();

        qDebug() << "TCP return uid: " << uid;
        qDebug() << "TCP return name: " << name;
        qDebug() << "TCP return nickname: " << nickname;
        qDebug() << "TCP return desc: " << desc;
        qDebug() << "TCP return gender: " << gender;
        qDebug() << "TCP return icon: " << icon;
        qDebug() << "TCP return email: " << email;

        SearchInfo searchinfo(uid, name, nickname, desc, gender, icon, email);
        auto searchInfoptr = std::make_shared<SearchInfo>(searchinfo);
        emit signal_search_user_success(searchInfoptr);
        qDebug() << "Search user successful";
    };

    // handle for MESSAGE_CHATSERVER_ADDFRIEND_ACK = 1008, // 服务端 → 发起方客户端：处理结果的反馈(你作为发起方, 目的是通知你好友申请是否成功)
    _handlers[RequestType::MESSAGE_CHATSERVER_ADDFRIEND_ACK] = [this](RequestType type, int len, QByteArray data) {
        /* ack作用是将经过服务器处理的from_uid : to_uid 加入到Usermanager 管理的 FriendRequstList*/
        Q_UNUSED(len);
        Q_UNUSED(type);
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull() || doc.isEmpty()) {
            qDebug() << "Invalid JSON response from chat server";
            return;
        }
        if (!doc.isObject()) {
            qDebug() << "Response is not a JSON object";
            return;
        }
        QJsonObject jsonObj = doc.object();
        if (!jsonObj.contains("error") && !jsonObj.contains("message")) {
            qDebug() << "Response does not contain 'error/message' field";
            return;
        }
        int errorCode = jsonObj.value("error").toInt();
        if (errorCode != static_cast<int>(ErrorCode::SUCCESS)) {
            qDebug() << "Add friend failed with error code:" << errorCode;
            qDebug() << "Response message: " << jsonObj.value("message").toString();
            return;
        }
        // Add friend successful
        int from_uid = jsonObj.value("from_uid").toInt();
        int to_uid = jsonObj.value("to_uid").toInt();
        qDebug() << "Add friend success with error code:" << errorCode;
        qDebug() << "Response message: " << jsonObj.value("message").toString();
        qDebug() << "Response from_uid: " << from_uid;
        qDebug() << "Response to_uid: " << to_uid;
    };

    // handle for MESSAGE_CHATSERVER_ADDFRIEND_PUSH = 1009, // 服务端 → 接收方客户端：转发好友申请通知(你作为接受方, 目的是通知你有新的好友申请)
    _handlers[RequestType::MESSAGE_CHATSERVER_ADDFRIEND_PUSH] = [this](RequestType type, int len, QByteArray data) {
        /* 你会收到服务器返回给你的申请人的信息*/
        Q_UNUSED(len);
        Q_UNUSED(type);
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isNull() || doc.isEmpty()) {
            qDebug() << "Invalid JSON response from chat server";
            return;
        }
        if (!doc.isObject()) {
            qDebug() << "Response is not a JSON object";
            return;
        }
        QJsonObject jsonObj = doc.object();
        if (!jsonObj.contains("error") && !jsonObj.contains("message")) {
            qDebug() << "Response does not contain 'error/message' field";
            return;
        }
        int errorCode = jsonObj.value("error").toInt();
        if (errorCode != static_cast<int>(ErrorCode::SUCCESS)) {
            qDebug() << "Receive friend request failed with error code:" << errorCode;
            qDebug() << "Response message: " << jsonObj.value("message").toString();
            return;
        }
        // Receive friend request successful
        int from_uid = jsonObj.value("uid").toInt();
        int from_gender = jsonObj.value("gender").toInt();
        QString from_name = jsonObj.value("name").toString();
        QString from_nickname = jsonObj.value("nickname").toString();
        QString from_icon = jsonObj.value("icon").toString();
        QString from_email = jsonObj.value("email").toString();
        QString from_description = jsonObj.value("desc").toString(); // TODO

        // 将好友申请信息添加到UserManager的好友申请列表中
        int self_uid = UserManager::GetInstance()->getUid();
        FriendListItemInfo requestInfo(self_uid, from_uid, from_name, from_description, from_icon, 0); // 0: (刚收到)未处理状态
        std::shared_ptr<FriendListItemInfo> newFriendInfoptr = std::make_shared<FriendListItemInfo>(requestInfo);
        emit signal_add_newFriendListItem(newFriendInfoptr);

        AddContactResponse addContactResponse(from_uid, from_name, from_nickname, from_icon, from_description, from_gender, from_email);
        auto addContactResponsePtr = std::make_shared<AddContactResponse>(addContactResponse);
        emit signal_add_contact_request_success(addContactResponsePtr); // 由FriendRequestPage接收

        // 联系人列表的添加联系人项红点亮起
        emit signal_addcontactlistitem_showRedPoint();
        qDebug() << "Receive friend request success with error code:" << errorCode;
        qDebug() << "Response message: " << jsonObj.value("message").toString();
        qDebug() << "Response from_uid: " << from_uid;
        qDebug() << "Response from_gender: " << from_gender;
        qDebug() << "Response from_name: " << from_name;
        qDebug() << "Response from_nickname: " << from_nickname;
        qDebug() << "Response from_icon: " << from_icon;
        qDebug() << "Response from_email: " << from_email;
        qDebug() << "Response from_description: " << from_description;
    };
}

void TcpManager::handleMessage(RequestType type, int len, QByteArray data)
{
    auto it = _handlers.find(type);
    if (it != _handlers.end()) {
        it.value()(type, len, data);
    } else {
        qDebug() << "No handler found for type:" << static_cast<int>(type);
    }
}

void TcpManager::slot_connect_to_chatserver(serverInfo info)
{
    _host = info.Host;
    _port = static_cast<quint16>(info.Port.toUShort());
    _socket->connectToHost(_host, _port);
    qDebug() << "Trying connecting to chat server at" << _host << ":" << _port;
}

void TcpManager::slot_send_data(RequestType type, const QString &jsondata)
{
    // Prepare the message in TLV format
    QByteArray jsonData = jsondata.toUtf8();
    _messageType = static_cast<quint16>(type);
    _messageLength = static_cast<quint16>(jsonData.size());
    QByteArray message;
    QDataStream stream(&message, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    // stream << _messageType << _messageLength << jsonData; //❗ 不要通过 QDataStream 写入 jsonData,会多写内容！！
    /*实际上会把 jsonData 作为 QByteArray 整体对象写入，而不是按原始字节附加，它会多写**自己的长度信息（一个 int32）**进去。*/
    stream << _messageType << _messageLength;
    message.append(jsonData); // ✅ 正确添加原始 JSON 字节流
    qDebug() << "Send message Type: " << _messageType;
    qDebug() << "Send message Length: " << _messageLength;
    qDebug() << "Send message Data: " << jsonData;

    if (_socket->state() != QTcpSocket::ConnectedState) {
        qDebug() << "Socket is not connected";
        emit signal_connect_to_chatserver_success(false);
        return;
    }
    qint64 bytesWritten = _socket->write(message);
    if (bytesWritten == -1) {
        qDebug() << "Failed to write data to socket";
    }
    else if (bytesWritten < message.size()) {
        qDebug() << "Partial data written to socket, expected" << message.size() << "but wrote" << bytesWritten;
    } else {
        qDebug() << "Data sent successfully to chat server";
    }
}
