#ifndef RESETPASSWORDDIALOG_H
#define RESETPASSWORDDIALOG_H

#include <QDialog>
#include <QMap>
#include "global.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "httpmanager.h"
namespace Ui {
class ResetPasswordDialog;
}

class ResetPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResetPasswordDialog(QWidget *parent = nullptr);
    ~ResetPasswordDialog();
private slots:
    bool checkUserValidation(const QString &text);

    bool checkEmailValidation(const QString &text);

    bool checkNewPasswordValidation(const QString &text);

    bool checkVerifyCodeValidation(const QString &text);

    void on_return_btn_clicked();

    void on_getVerifyCode_btn_clicked();

    void on_confirm_btn_clicked();

    void slot_reset_password_handle(RequestType type, QString response, ErrorCode error);
private:
    Ui::ResetPasswordDialog *ui;
    QMap<RequestType, std::function<void(const QJsonObject&)>> _handlers;
private:
    void showTip(QString str, bool isError);
    void initialHttpHandlers();

signals:
    void signal_switchto_login();
};

#endif // RESETPASSWORDDIALOG_H
