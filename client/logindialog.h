#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QPainter>
#include <QPainterPath>
#include "global.h"
#include <QRegularExpression>
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include "httpmanager.h"

/******************************************************************************************
 *  1.initHttpHandlers
 *  2.on_login_btn_clicked(准备json)
 *  3.HttpManager::GetInstance()->PostHttpRequest()     finished--->signal_login_module_finished
 *   (通过状态服务器负载均衡获取到了压力最小的聊天服务器地址和token)
 *  4.signal_login_module_finished----> slot_login_module_handle  ------> emit signal_connect_to_tcpserver(info);
 *  (通过tcpserver连接到聊天服务器，此时使用长连接)
********************************************************************************************/

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;
    QMap<RequestType, std::function<void (const QJsonObject&)>> _handlers;
private:
    void InitPixmap(); // 将资源图片裁剪成圆角图标，并显示在界面上某个标签控件中
    bool checkEmailValidation();
    bool checkPasswordValidation();
    void showTip(const QString& text, bool isError = false);
    void initHttpHandlers();
    void setWidgetsEnable(bool enable);
private slots:
    void slot_forget_password();
    void on_login_btn_clicked();
    void slot_login_module_handle(RequestType type, QString response, ErrorCode error);
signals:
    void switchToRegister();
    void switchToResetPassword();
    void signal_connect_to_tcpserver(serverInfo);
};

#endif // LOGINDIALOG_H
