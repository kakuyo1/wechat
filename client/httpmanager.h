#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H
#include "singleton.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <memory>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include "global.h"
#include <QDebug>

class HttpManager : public QObject, public Singleton<HttpManager>,
                    public std::enable_shared_from_this<HttpManager>
{
    Q_OBJECT
    friend class Singleton<HttpManager>;
public:
    ~HttpManager() = default; // must be public to allow Singleton to call it
    void PostHttpRequest(QUrl url, QJsonObject jsonObj, RequestType type, Module module);
private:
    HttpManager();

private:
    QNetworkAccessManager *networkManager;

private slots:
    void slot_http_request_finished(RequestType type,QString response, ErrorCode error, Module module);
signals:
    void signal_http_request_finished(RequestType type,QString response, ErrorCode error, Module module);
    void signal_register_module_finished(RequestType type, QString response, ErrorCode error);
    void signal_reset_module_finished(RequestType type, QString response, ErrorCode error);
};

#endif // HTTPMANAGER_H
