#include "tcpmanager.h"

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
    _handlers[RequestType::TYPE_LOGIN_CHAT] = [this](RequestType type, int len, QByteArray data) {
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
            emit signal_login_failed();
            return;
        }
        // Authentication successful
        qDebug() << "Login authentication successful";
        emit signal_switchto_chatdialog();
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
    stream << _messageType << _messageLength << jsonData;
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
