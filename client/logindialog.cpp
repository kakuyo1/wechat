#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    // Connect the signal to the slot for showing the register dialog
    connect(ui->register_btn, &QPushButton::clicked, this, &LoginDialog::switchToRegister);

    // Connect the signal to the slot for showing the reset dialog
    connect(ui->forget_label, &ForgetLabel::clicked, this, &LoginDialog::slot_forget_password);

    // set the password mode and placeholders for input fields
    ui->password_lineEdit->setEchoMode(QLineEdit::Password);
    ui->user_lineEdit->setPlaceholderText("请输入用户名");
    ui->password_lineEdit->setPlaceholderText("请输入密码");

    // set cursor for forget_label and connect the clicked signal to a slot
    ui->forget_label->setCursor(Qt::PointingHandCursor);
    ui->forget_label->setStyleSheet("QLabel { color: black;}" "QLabel:hover { color: rgb(42, 112, 241); }");
}

LoginDialog::~LoginDialog()
{
    qDebug() << "LoginDialog destructor called";
    delete ui;
}

void LoginDialog::slot_forget_password()
{
    qDebug() << "Forget password clicked";
    emit switchToResetPassword();
}

