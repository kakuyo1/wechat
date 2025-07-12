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
};

#endif // TCPMANAGER_H
