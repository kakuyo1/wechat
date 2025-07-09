#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmanager.h"
RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);

    // set the property for error_tip
    ui->error_tip->setProperty("state", "normal");
    repolish(ui->error_tip);

    // set the password mode and placeholders for input fields
    ui->password_lineEdit->setEchoMode(QLineEdit::Password);
    ui->confirm_lineEdit->setEchoMode(QLineEdit::Password);
    ui->user_lineEdit->setPlaceholderText("请输入用户名");
    ui->email_lineEdit->setPlaceholderText("请输入邮箱");
    ui->password_lineEdit->setPlaceholderText("请输入密码");
    ui->confirm_lineEdit->setPlaceholderText("请确认密码");

    // connect the signal from HttpManager to the slot for handling register module finished
    connect(HttpManager::GetInstance().get(), &HttpManager::signal_register_module_finished,
            this, &RegisterDialog::slot_register_module_handle);
    // initialize the http handlers
    initialHttpHandlers();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_getVerifyCode_btn_clicked()
{
    // check if the email is valid by using regular expression
    auto email = ui->email_lineEdit->text();
    static QRegularExpression emailRegex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = emailRegex.match(email).hasMatch();

    if (!match) { // when wrong email format
        showTip("请输入正确的邮箱格式", true);
        return;
    }

    // when email format is correct(send http request to get the verify code)
    QJsonObject jsonObj;
    jsonObj.insert("email", email);
    HttpManager::GetInstance()->PostHttpRequest(QUrl(gate_url_prefix + "/get_verifycode"), jsonObj,
                                                RequestType::TYPE_GET_VERIFY_CODE, Module::MODULE_REGISTER);
}

void RegisterDialog::slot_register_module_handle(RequestType type, QString response, ErrorCode error)
{
    if (error != ErrorCode::SUCCESS) {
        showTip("网络请求错误", true);
        return;
    }
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    if (doc.isNull() || doc.isEmpty()) {
        showTip("服务器响应错误", true);
        return;
    }
    if (!doc.isObject()) {
        showTip("服务器响应格式错误", true);
        return;
    }
    QJsonObject jsonObj = doc.object();
    // handle the jsonObj
    _handlers[type](jsonObj);
    return;
}

void RegisterDialog::showTip(QString str, bool isError)
{
    ui->error_tip->setText(str);
    ui->error_tip->setProperty("state", isError ? "error" : "normal");
    repolish(ui->error_tip);
}

void RegisterDialog::initialHttpHandlers()
{
    // get verify_code handler
    _handlers[RequestType::TYPE_GET_VERIFY_CODE] = [this](QJsonObject &jsonObj) {
        int error = jsonObj.value("error").toInt();
        if (error != static_cast<int>(ErrorCode::SUCCESS)) {
            showTip("获取验证码失败", true);
            return;
        }
        auto email = jsonObj.value("email").toString(); // from server response
        //  debug
        // qDebug() << email;
        // qDebug() << jsonObj.value("message").toString();
        // qDebug() << jsonObj.value("code").toString();
        // qDebug() << error;
        showTip("验证码已发送,注意查收", false);
    };

    // register handler
    _handlers[RequestType::TYPE_REGISTER] = [this](QJsonObject &jsonObj) {
        int error = jsonObj.value("error").toInt();
        if (error == static_cast<int>(ErrorCode::ERROR_EXISTING_USER)) {
            showTip("用户已存在", true);
            // qDebug() << jsonObj.value("message").toString();
            // qDebug() << error;
            return;
        }
        if (error != static_cast<int>(ErrorCode::SUCCESS)) {
            showTip("注册失败, 请稍后重试", true);
            // qDebug() << jsonObj.value("message").toString();
            // qDebug() << error;
            return;
        }
        showTip("注册成功", false);
    };
}


void RegisterDialog::on_confirm_btn_clicked()
{
    // check inputs
    if (ui->user_lineEdit->text().isEmpty()) {
        showTip("用户名不能为空", true);
        return;
    }

    if (ui->email_lineEdit->text().isEmpty()) {
        showTip("邮箱不能为空", true);
        return;
    }

    if (ui->password_lineEdit->text().isEmpty()) {
        showTip("密码不能为空", true);
        return;
    }

    if (ui->confirm_lineEdit->text().isEmpty()) {
        showTip("请确认密码", true);
        return;
    }

    if (ui->password_lineEdit->text() != ui->confirm_lineEdit->text()) {
        showTip("两次输入密码不一致", true);
        return;
    }

    if (ui->verifyCode_lineEdit->text().isEmpty()) {
        showTip("验证码不能为空", true);
        return;
    }
    // all inputs are valid, create a json object and send http request to register
    QJsonObject jsonObj;
    jsonObj["user"] = ui->user_lineEdit->text();
    jsonObj["email"] = ui->email_lineEdit->text();
    jsonObj["password"] = ui->password_lineEdit->text();
    jsonObj["confirm"] = ui->confirm_lineEdit->text();
    jsonObj["verifycode"] = ui->verifyCode_lineEdit->text();
    HttpManager::GetInstance()->PostHttpRequest(QUrl(gate_url_prefix + "/user_register"), jsonObj,
                                                RequestType::TYPE_REGISTER, Module::MODULE_REGISTER);
}

