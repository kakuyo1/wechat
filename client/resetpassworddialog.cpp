#include "resetpassworddialog.h"
#include "ui_resetpassworddialog.h"

ResetPasswordDialog::ResetPasswordDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResetPasswordDialog)
{
    ui->setupUi(this);

    // connect the signals to the validation slots
    connect(ui->user_lineEdit, &QLineEdit::textChanged, this, &ResetPasswordDialog::checkUserValidation);
    connect(ui->email_lineEdit, &QLineEdit::textChanged, this, &ResetPasswordDialog::checkEmailValidation);
    connect(ui->newPassword_lineEdit, &QLineEdit::textChanged, this, &ResetPasswordDialog::checkNewPasswordValidation);
    connect(ui->verifyCode_lineEdit, &QLineEdit::textChanged, this, &ResetPasswordDialog::checkVerifyCodeValidation);

    // connect the signal from httpmanager to the handler
    connect(HttpManager::GetInstance().get(), &HttpManager::signal_reset_module_finished,
            this, &ResetPasswordDialog::slot_reset_password_handle);

    // set property state(normal,error) for error_tip
    ui->error_tip->setProperty("state", "normal");
    repolish(ui->error_tip);

    initialHttpHandlers();
}

ResetPasswordDialog::~ResetPasswordDialog()
{
    qDebug() << "ResetPasswordDialog destructor called";
    delete ui;
}

bool ResetPasswordDialog::checkUserValidation(const QString &text)
{
    if (text.isEmpty()) {
        showTip("用户名不能为空", true);
        return false;
    }

    static QRegularExpression userRegex(R"(^[a-zA-Z0-9_]{3,16}$)");
    bool match = userRegex.match(text).hasMatch();
    if (!match) {
        showTip("用户名格式不正确", true);
        return false;
    } else {
        showTip("", false); // Clear the tip if the username is valid
        return true;
    }
}

bool ResetPasswordDialog::checkEmailValidation(const QString &text)
{
    if (text.isEmpty()) {
        showTip("邮箱不能为空", true);
        return false;
    }

    static QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    bool match = emailRegex.match(text).hasMatch();
    if (!match) {
        showTip("请输入正确的邮箱格式", true);
        return false;
    } else {
        showTip("", false); // Clear the tip if the email is valid
        return true;
    }
}

bool ResetPasswordDialog::checkNewPasswordValidation(const QString &text)
{
    if (text.isEmpty()) {
        showTip("新密码不能为空", true);
        return false;
    }

    // Check password format
    static QRegularExpression passwordRegex(R"(^[a-zA-Z0-9_]{6,64}$)");
    bool match = passwordRegex.match(text).hasMatch();
    if (!match) {
        showTip("新密码格式不正确", true);
        return false;
    } else {
        showTip("", false); // Clear the tip if the new password is valid
        return true;
    }
}

bool ResetPasswordDialog::checkVerifyCodeValidation(const QString &text)
{
    if (text.isEmpty()) {
        showTip("验证码不能为空", true);
        return false;
    }

    // Check if the verification code is numeric and 6 digits long
    static QRegularExpression codeRegex(R"(^\d{6}$)");
    bool match = codeRegex.match(text).hasMatch();
    if (!match) {
        showTip("验证码格式不正确", true);
        return false;
    } else {
        showTip("", false); // Clear the tip if the verification code is valid
        return true;
    }
}

void ResetPasswordDialog::on_return_btn_clicked()
{
    emit signal_switchto_login();
}

void ResetPasswordDialog::showTip(QString str, bool isError)
{
    ui->error_tip->setText(str);
    ui->error_tip->setProperty("state", isError ? "error" : "normal");
    repolish(ui->error_tip);
}

void ResetPasswordDialog::initialHttpHandlers()
{
    // handler for get_verifycode
    _handlers[RequestType::TYPE_GET_VERIFY_CODE] = [this](const QJsonObject &jsonObj) {
        if (jsonObj.contains("error") && jsonObj.contains("email") && jsonObj.contains("code") && jsonObj.contains("message")) {
            int error = jsonObj["error"].toInt();
            if (error != static_cast<int>(ErrorCode::SUCCESS)) {
                showTip("获取验证码失败", true);
                return;
            }
            auto email = jsonObj["email"].toString();
            auto code = jsonObj["code"].toString();
            auto message = jsonObj["message"].toString();
            showTip("验证码已发送,注意查收", false);
            qDebug() << "Email:" << email;
            qDebug() << "Code:" << code;
            qDebug() << "Message:" << message;
            qDebug() << "Error:" << error;
        } else {
            showTip("服务器响应数据不完整", true);
            return;
        }
    };

    // handler for reset password
    _handlers[RequestType::TYPE_RESETPASSWORD] = [this](const QJsonObject &jsonObj) {
        if (jsonObj.contains("error") && jsonObj.contains("message")) {
            int error = jsonObj["error"].toInt();
            if (error == static_cast<int>(ErrorCode::ERROR_INVALID_VERIFY_CODE)) {
                showTip("验证码不正确", true);
                qDebug() << "Error:" << error;
                qDebug() << "Message" << jsonObj["message"].toString();
                return;
            }
            if (error == static_cast<int>(ErrorCode::ERROR_USER_EMAIL_MISMATCH)) {
                showTip("用户名和邮箱不匹配", true);
                qDebug() << "Error:" << error;
                qDebug() << "Message" << jsonObj["message"].toString();
                return;
            }
            if (error != static_cast<int>(ErrorCode::SUCCESS)) {
                showTip("密码重置失败", true);
                qDebug() << "Error:" << error;
                qDebug() << "Message" << jsonObj["message"].toString();
                return;
            }
            showTip("密码重置成功, 点击返回回到登陆页面", false);
            auto message = jsonObj["message"].toString();
            qDebug() << "Message:" << message;
            qDebug() << "Error:" << error;
        }
    };
}


void ResetPasswordDialog::on_getVerifyCode_btn_clicked()
{
    // check if the email is valid by using regular expression
    auto email = ui->email_lineEdit->text();
    static QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    bool match = emailRegex.match(email).hasMatch();

    if (!match) { // when wrong email format
        showTip("请输入正确的邮箱格式", true);
        return;
    }

    // when email format is correct(send http request to get the verify code)
    QJsonObject jsonObj;
    jsonObj.insert("email", email);
    HttpManager::GetInstance()->PostHttpRequest(QUrl(gate_url_prefix + "/get_verifycode"), jsonObj,
                                                RequestType::TYPE_GET_VERIFY_CODE, Module::MODULE_RESETPASSWORD);
}


void ResetPasswordDialog::on_confirm_btn_clicked()
{
    // check inputs
    if (!checkUserValidation(ui->user_lineEdit->text()) ||
        !checkEmailValidation(ui->email_lineEdit->text()) ||
        !checkNewPasswordValidation(ui->newPassword_lineEdit->text()) ||
        !checkVerifyCodeValidation(ui->verifyCode_lineEdit->text())) {
        return; // if any validation fails, return early
    }

    // all inputs are valid, create a json object and send http request to reset password
    QJsonObject jsonObj;
    jsonObj["user"] = ui->user_lineEdit->text();
    jsonObj["email"] = ui->email_lineEdit->text();
    jsonObj["newpassword"] = ui->newPassword_lineEdit->text();
    jsonObj["verifycode"] = ui->verifyCode_lineEdit->text();

    HttpManager::GetInstance()->PostHttpRequest(QUrl(gate_url_prefix + "/reset_password"), jsonObj,
                                                RequestType::TYPE_RESETPASSWORD, Module::MODULE_RESETPASSWORD);
}

void ResetPasswordDialog::slot_reset_password_handle(RequestType type, QString response, ErrorCode error)
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

