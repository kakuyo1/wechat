#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QRegularExpression>
#include "global.h"
#include <QMap>
#include <QTimer>
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_getVerifyCode_btn_clicked();
    void slot_register_module_handle(RequestType type, QString response, ErrorCode error);
    void on_confirm_btn_clicked();

    void validateUserName(const QString& text);
    void validateEmail(const QString& text);
    void validatePassword(const QString& text);
    void validateConfirm(const QString& text);

    void on_return_pushButton_clicked();

    void on_cancel_btn_clicked();

private:
    Ui::RegisterDialog *ui;
    QMap<RequestType, std::function<void (QJsonObject&)>> _handlers;
    QTimer* countDownTimer;
    int countDownTime;
private:
    void showTip(QString str, bool isError);
    void initialHttpHandlers();
signals:
    void signal_switchto_login();
};

#endif // REGISTERDIALOG_H
