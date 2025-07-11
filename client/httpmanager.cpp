#include "httpmanager.h"

HttpManager::HttpManager() : networkManager(new QNetworkAccessManager(this)) {
    connect(this, &HttpManager::signal_http_request_finished,
            this, &HttpManager::slot_http_request_finished);
}

void HttpManager::PostHttpRequest(QUrl url, QJsonObject jsonObj, RequestType type, Module module)
{
    // create and process the request
    QNetworkRequest request(url);

    QJsonDocument doc(jsonObj);
    QByteArray data = doc.toJson();
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(data.length()));

    auto self = shared_from_this();
    QNetworkReply *reply = networkManager->post(request, data); //send the POST request

    // check if the reply is valid
    connect(reply, &QNetworkReply::finished, [self, reply, type, module](){
        if (reply->error() != QNetworkReply::NoError) { // when wrong
            qDebug() << "HTTP Request Error(HttpManager::PostHttpRequest):" << reply->errorString();
            emit self->signal_http_request_finished(type, "", ErrorCode::ERROR_NETWORK, module);
            reply->deleteLater();
            return;
        }

        // when the request is successful
        QString response = reply->readAll();
        emit self->signal_http_request_finished(type, response, ErrorCode::SUCCESS, module);
        reply->deleteLater();
        return;
    });
}

void HttpManager::slot_http_request_finished(RequestType type, QString response, ErrorCode error, Module module)
{
    if (module == Module::MODULE_REGISTER) {
        emit signal_register_module_finished(type, response, error);
    }
    if (module == Module::MODULE_RESETPASSWORD) {
        emit signal_reset_module_finished(type, response, error);
    }
    if (module == Module::MODULE_LOGIN) {
        emit signal_login_module_finished(type, response, error);
    }
}
