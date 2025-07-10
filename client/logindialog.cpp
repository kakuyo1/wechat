#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    // Connect the signal to the slot for showing the register dialog
    connect(ui->register_btn, &QPushButton::clicked, this, &LoginDialog::switchToRegister);

    // set the password mode and placeholders for input fields
    ui->password_lineEdit->setEchoMode(QLineEdit::Password);
    ui->user_lineEdit->setPlaceholderText("请输入用户名");
    ui->password_lineEdit->setPlaceholderText("请输入密码");
}

LoginDialog::~LoginDialog()
{
    qDebug() << "LoginDialog destructor called";
    delete ui;
}

