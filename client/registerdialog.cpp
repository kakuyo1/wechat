#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmanager.h"
RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::RegisterDialog),
      countDownTimer(new QTimer(this)),
      countDownTime(5)
{
    ui->setupUi(this);

    // set deafult page
    ui->stackedWidget->setCurrentIndex(0); // set to page_1

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

    //  check lineEdit valid in time
    connect(ui->user_lineEdit, &QLineEdit::textChanged, this, &RegisterDialog::validateUserName);
    connect(ui->email_lineEdit, &QLineEdit::textChanged, this, &RegisterDialog::validateEmail);
    connect(ui->password_lineEdit, &QLineEdit::textChanged, this, &RegisterDialog::validatePassword);
    connect(ui->confirm_lineEdit, &QLineEdit::textChanged, this, &RegisterDialog::validateConfirm);

    // set visiable icon cursor and change the state if visiable by receive the signal from ClickableLabel
    ui->password_visiable_label->setCursor(Qt::PointingHandCursor);
    ui->confirm_visiable_lable->setCursor(Qt::PointingHandCursor);

    connect(ui->password_visiable_label, &ClickableLabel::clicked, this, [this](){
        if (this->ui->password_visiable_label->getState()) { // when password is visiable
            ui->password_lineEdit->setEchoMode(QLineEdit::Normal);
        } else {
            ui->password_lineEdit->setEchoMode(QLineEdit::Password);
        }
    });
    connect(ui->confirm_visiable_lable, &ClickableLabel::clicked, this, [this](){
        if (this->ui->confirm_visiable_lable->getState()) { // when confirm password is visiable
            ui->confirm_lineEdit->setEchoMode(QLineEdit::Normal);
        } else {
            ui->confirm_lineEdit->setEchoMode(QLineEdit::Password);
        }
    });

    // connect the timer to the slot for handling countdown
    connect(countDownTimer, &QTimer::timeout, this, [this](){
        if (countDownTime == 0) {
            countDownTimer->stop();
            emit signal_switchto_login();
            return;
        }
        countDownTime--;
        ui->countdown_label->setText("注册成功，" + QString::number(countDownTime) + "秒后自动返回登录界面");
    });

    // connect the signal from HttpManager to the slot for handling register module finished
    connect(HttpManager::GetInstance().get(), &HttpManager::signal_register_module_finished,
            this, &RegisterDialog::slot_register_module_handle);
    // initialize the http handlers
    initialHttpHandlers();
}

RegisterDialog::~RegisterDialog()
{
    qDebug() << "RegisterDialog destructor called";
    delete ui;
}

void RegisterDialog::on_getVerifyCode_btn_clicked()
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
        if (error == static_cast<int>(ErrorCode::ERROR_EXISTING_EMAIL)) {
            showTip("邮箱已注册", true);
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
        // switch to page_2
        countDownTimer->stop();
        ui->stackedWidget->setCurrentWidget(ui->page_2); // set to page_2
        countDownTimer->start(1000);
    };
}


void RegisterDialog::on_confirm_btn_clicked()
{
    // check inputs
    validateUserName(ui->user_lineEdit->text());
    validatePassword(ui->password_lineEdit->text());
    validateConfirm(ui->confirm_lineEdit->text());
    validateEmail(ui->email_lineEdit->text());

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

void RegisterDialog::validateUserName(const QString &text)
{
    if (text.isEmpty()) {
        showTip("用户名不能为空", true);
        return;
    }

    showTip("", false); // Clear the tip if the username is valid
    static QRegularExpression userRegex(R"(^[a-zA-Z0-9_]{3,16}$)");
    bool match = userRegex.match(text).hasMatch();
    if (!match) {
        showTip("用户名格式不正确", true);
    } else {
        showTip("", false);
    }
}

void RegisterDialog::validateEmail(const QString &text)
{
    if (text.isEmpty()) {
        showTip("邮箱不能为空", true);
        return;
    }

    static QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    bool match = emailRegex.match(text).hasMatch();
    if (!match) {
        showTip("请输入正确的邮箱格式", true);
    } else {
        showTip("", false); // Clear the tip if the email is valid
    }
}

void RegisterDialog::validatePassword(const QString &text)
{
    if (text.isEmpty()) {
        showTip("密码不能为空", true);
        return;
    }

    // 1. 先检查密码格式
    static QRegularExpression passwordRegex(R"(^[a-zA-Z0-9_]{6,64}$)");
    bool match = passwordRegex.match(text).hasMatch();
    if (!match) {
        showTip("密码格式不正确", true);
        return; // 格式错误时直接返回，不再检查一致性
    }

    // 2. 格式正确后，再检查确认密码是否匹配（如果 confirm 非空）
    QString confirmText = ui->confirm_lineEdit->text();
    if (!confirmText.isEmpty() && text != confirmText) {
        showTip("两次输入密码不一致", true);
    } else {
        showTip("", false);
    }
}

void RegisterDialog::validateConfirm(const QString &text)
{
    if (text.isEmpty()) {
        showTip("请确认密码", true);
        return;
    }

    if (text != ui->password_lineEdit->text()) {
        showTip("两次输入密码不一致", true);
    } else {
        showTip("", false); // Clear the tip if the confirmation is valid
    }
}


void RegisterDialog::on_return_pushButton_clicked()
{
    countDownTimer->stop();
    emit signal_switchto_login();
}

