#ifndef TCPMANAGER_H
#define TCPMANAGER_H
#include <QObject>
#include <QTcpSocket>
#include <QMap>
#include <QJsonDocument>
#include <QJsonObject>
#include "global.h"
#include "singleton.h"
#include <memory>

/***********************************************************************
 * loginDialog在收到StatusServer返回的（uid，token，port，host）后构建聊天服务器SeverInfo
发射信号emit signal_connect_to_chatserver(info);给TcpManager
TcpManager则connect(signal_connect_to_chatserver, slot_connect_to_chatserver)
TcpManager用QTcpSocket->ConnectToHost()实现slot_connect_to_chatserver连接到StatusServer返回的指定聊天服务器

同时TcpManager监听TcpSocket->ConnectToHost()可能发出的connected和errorOccured信号
connected信号如果被捕捉继续发送emit sig_connectToChat_success(true)
emit sig_connectToChat_success(true)被Logindialog监听connect(sig_connectToChat_success, slot_connectToChat_success)
Logindialog::slot_connectToChat_success中使用uid和token在成功连接聊天服务器后验证身份，也就是
发送signal_send_data(RequestType, QJson_String), signal_send_data信号被TcpManager::slot_send_data监听
（它负责把RequestType, QJson_String发给聊天服务器），接着聊天服务器处理，等待readyRead信号

errorOccured信号如果被捕捉继续发送emit sig_connectToChat_success(flase)
同时TcpManager监听disconnected信号,打印信息即可

同时TcpManager监听readyRead信号（来自chatServer中转的信息（这是就是验证登陆信息的返回）），
按照TLV处理粘包问题在根据RequestType来调用InitHandlers里注册的回调函数(TYPE_LOGIN_CHATSERVER)
来收取信息(登陆请求就是发送信息,触发readyRead),返回error不是SUCCESS或者json解析错误，信息不完整等都登录失败，
发送signal_login_failed信号(交给loginDialog::slot_login_failed处理)，否则发送signal_swtichto_chatdialog信号(交给mainwindow处理)
***********************************************************************/

class TcpManager : public QObject, public Singleton<TcpManager>, public std::enable_shared_from_this<TcpManager>
{
    Q_OBJECT
    friend class Singleton<TcpManager>;
public:
    ~TcpManager() = default;
private:
    TcpManager();
    void initHandlers();
    void handleMessage(RequestType type, int len, QByteArray data); // TLV  format
public slots:
    void slot_connect_to_chatserver(serverInfo info);
    void slot_send_data(RequestType type, const QString& jsondata);
private:
    QTcpSocket* _socket;
    QMap<RequestType, std::function<void(RequestType type, int len, QByteArray data)>> _handlers;
    QString _host;
    quint16 _port;
    quint16 _messageLength;
    quint16 _messageType;
    QByteArray _messageBuffer;
    bool _hasIncompleteBody;
signals:
    void signal_connect_to_chatserver_success(bool success);
    void signal_login_failed();
    void signal_switchto_chatdialog();
    void signal_send_data(RequestType type, const QString& jsondata);
    void signal_login_failed_online_already();
};

#endif // TCPMANAGER_H
