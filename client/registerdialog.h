#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QRegularExpression>
#include "global.h"
#include <QMap>
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
private:
    Ui::RegisterDialog *ui;
    QMap<RequestType, std::function<void (QJsonObject&)>> _handlers;
private:
    void showTip(QString str, bool isError);
    void initialHttpHandlers();
};

#endif // REGISTERDIALOG_H
