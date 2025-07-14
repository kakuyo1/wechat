#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::LoginDialog),
      _hasSentAuthenticationRequest(false)
{
    ui->setupUi(this);
    InitPixmap();

    // set property for error_tip
    ui->error_tip->setProperty("state", "normal");
    repolish(ui->error_tip);

    // Connect the signal to the slot for showing the register dialog
    connect(ui->register_btn, &QPushButton::clicked, this, &LoginDialog::switchToRegister);

    // Connect the signal to the slot for showing the reset dialog
    connect(ui->forget_label, &ForgetLabel::clicked, this, &LoginDialog::slot_forget_password);

    // Connect the signal to the slot for check inputs in time
    connect(ui->email_lineEdit, &QLineEdit::textChanged, this, &LoginDialog::checkEmailValidation);
    connect(ui->password_lineEdit, &QLineEdit::textChanged, this, &LoginDialog::checkPasswordValidation);

    // connect the signal from HttpManager to the handler
    connect(HttpManager::GetInstance().get(), &HttpManager::signal_login_module_finished,
            this, &LoginDialog::slot_login_module_handle);

    // connect the signal(connect_to_chatserver) to the slot in TcpManager
    connect(this, &LoginDialog::signal_connect_to_chatserver, TcpManager::GetInstance().get(), &TcpManager::slot_connect_to_chatserver);

    // connect the signas(connect to chatserver success or failed) to the slots
    connect(TcpManager::GetInstance().get(), &TcpManager::signal_connect_to_chatserver_success,
            this, &LoginDialog::slot_connect_to_chatserver_success);

    // connect the signal from TcpManager(login failed authentication from chatserver) to the slot
    connect(TcpManager::GetInstance().get(), &TcpManager::signal_login_failed,
            this, &LoginDialog::slot_login_failed);

    // connect the signal swtichto Chatdialog to the slot
    connect(TcpManager::GetInstance().get(), &TcpManager::signal_switchto_chatdialog,
            this, [this](){
        this->showTip("验证成功，正在切换到聊天页面...", false);
    });

    // connect the signal signal_login_failed_online_already to the slot
    connect(TcpManager::GetInstance().get(), &TcpManager::signal_login_failed_online_already,
            this, [this](){
        showTip("用户已经在线，请勿重复登录", true);
        _hasSentAuthenticationRequest = false;
        setWidgetsEnable(true);
    });

    // set the password mode and placeholders for input fields
    ui->password_lineEdit->setEchoMode(QLineEdit::Password);
    ui->email_lineEdit->setPlaceholderText("请输入邮箱");
    ui->password_lineEdit->setPlaceholderText("请输入密码");

    // set cursor for forget_label and connect the clicked signal to a slot
    ui->forget_label->setCursor(Qt::PointingHandCursor);
    ui->forget_label->setStyleSheet("QLabel { color: black;}" "QLabel:hover { color: rgb(42, 112, 241); }");

    initHttpHandlers();
}

LoginDialog::~LoginDialog()
{
    qDebug() << "LoginDialog destructor called";
    delete ui;
}

void LoginDialog::InitPixmap()
{
    QPixmap origin_pixmap(":/images/wechat.png");
    origin_pixmap = origin_pixmap.scaled(ui->icon_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QPixmap rounded_pixmap(origin_pixmap.size());
    rounded_pixmap.fill(Qt::transparent);

    QPainter painter(&rounded_pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QPainterPath path;
    path.addRoundedRect(rounded_pixmap.rect(), 10, 10); // based on rounded_pixmap
    painter.setClipPath(path);

    painter.drawPixmap(0, 0, origin_pixmap);

    ui->icon_label->setPixmap(rounded_pixmap);
}

bool LoginDialog::checkEmailValidation()
{
    QString email = ui->email_lineEdit->text();
    if (email.isEmpty()) {
        showTip("邮箱不能为空", true);
        return false;
    }
    // Check email format using regex
    static QRegularExpression emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    bool match = emailRegex.match(email).hasMatch();
    if (!match) {
        showTip("请输入正确的邮箱格式", true);
        return false;
    } else {
        showTip("", false); // Clear the tip if the email is valid
        return true;
    }
}

bool LoginDialog::checkPasswordValidation()
{
    QString password = ui->password_lineEdit->text();
    if (password.isEmpty()) {
        showTip("密码不能为空", true);
        return false;
    }
    // Check password format using regex
    static QRegularExpression passwordRegex(R"(^[a-zA-Z0-9_]{6,64}$)");
    bool match = passwordRegex.match(password).hasMatch();
    if (!match) {
        showTip("密码格式不正确", true);
        return false;
    } else {
        showTip("", false); // Clear the tip if the password is valid
        return true;
    }
}

void LoginDialog::showTip(const QString& text, bool isError)
{
    ui->error_tip->setText(text);
    ui->error_tip->setProperty("state", isError ? "error" : "normal");
    repolish(ui->error_tip);
}

void LoginDialog::initHttpHandlers()
{
    // get the chat server address and token from the state server
    _handlers[RequestType::TYPE_LOGIN] = [this](const QJsonObject& jsonObject) {
        int error = jsonObject["error"].toInt();
        if (error != static_cast<int>(ErrorCode::SUCCESS)) {
            showTip("无法获取聊天服务器地址", true);
            qDebug() << "Error code:" << error;
            qDebug() << "Message:" << jsonObject["message"].toString();
            return;
        }

        if (!jsonObject.contains("error") || !jsonObject.contains("message") || !jsonObject.contains("token")
            || !jsonObject.contains("uid") || !jsonObject.contains("host") || !jsonObject.contains("port")) {
            showTip("状态服务器响应数据不完整", true);
            qDebug() << "Error code:" << error;
            qDebug() << "Message:" << jsonObject["message"].toString();
            return;
        }

        QString message = jsonObject["message"].toString();
        QString token = jsonObject["token"].toString();
        QString host = jsonObject["host"].toString();
        QString port = jsonObject["port"].toString();
        int uid = jsonObject["uid"].toInt();

        qDebug() << "Get ChatServer address successful, message:" << message;
        qDebug() << "Error code:" << error;
        qDebug() << "Token:" << token;
        qDebug() << "Host:" << host;
        qDebug() << "Port:" << port;
        qDebug() << "Uid:" << uid;

        serverInfo info;
        info.Host = host;
        info.Port = port;
        info.Token = token;
        info.Uid = uid;

        _uid = QString::number(uid);
        _token = token;

        showTip("正在尝试连接聊天服务器...", false);
        emit signal_connect_to_chatserver(info);
    };
}

void LoginDialog::setWidgetsEnable(bool enable)
{
    ui->login_btn->setEnabled(enable);
    ui->register_btn->setEnabled(enable);
    ui->email_lineEdit->setEnabled(enable);
    ui->password_lineEdit->setEnabled(enable);
    ui->forget_label->setEnabled(enable);
}

void LoginDialog::slot_forget_password()
{
    qDebug() << "Forget password clicked";
    emit switchToResetPassword();
}


void LoginDialog::on_login_btn_clicked()
{
    // Get the username and password from the input fields
    QString email = ui->email_lineEdit->text();
    QString password = ui->password_lineEdit->text();

    // Validate the email and password
    if (!checkEmailValidation() || !checkPasswordValidation()) {
        return;
    }
    // Create a JSON object with the login data
    QJsonObject jsonObj;
    jsonObj.insert("email", email);
    jsonObj.insert("password", password);

    // Send the login request using HttpManager
    HttpManager::GetInstance()->PostHttpRequest(QUrl(gate_url_prefix + "/user_login"), jsonObj,
                                                RequestType::TYPE_LOGIN, Module::MODULE_LOGIN);
}

void LoginDialog::slot_login_module_handle(RequestType type, QString response, ErrorCode error)
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

void LoginDialog::slot_connect_to_chatserver_success(bool success)
{
    if (_hasSentAuthenticationRequest) {
        // If an authentication request has already been sent, do not send another one(防止用户频繁点击)
        qDebug() << "Authentication request already sent. Skipping duplicate.";
        return;
    }
    if (success) {
        _hasSentAuthenticationRequest = true;
        setWidgetsEnable(false);
        QJsonObject jsonObj;
        jsonObj.insert("uid", _uid.toInt());
        jsonObj.insert("token", _token);
        QString jsonData = QJsonDocument(jsonObj).toJson(QJsonDocument::Compact);
        // Send the authentication request to the chat server
        qDebug() << "Authentication request sent to chat server with UID:" << _uid << "and Token:" << _token;
        emit TcpManager::GetInstance()->signal_send_data(RequestType::TYPE_LOGIN_CHAT, jsonData);
    } else {
        showTip("连接聊天服务器失败，请重试", true);
        _hasSentAuthenticationRequest = false;
        setWidgetsEnable(true);
    }
}

void LoginDialog::slot_login_failed()
{
    showTip("登录失败，请检查邮箱和密码", true);
    _hasSentAuthenticationRequest = false;
    setWidgetsEnable(true);
}

