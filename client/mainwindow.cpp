#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , loginDialog(new LoginDialog(this))
{
    ui->setupUi(this);
    // Initialize the login dialog and set it as the central widget
    setCentralWidget(loginDialog);
    loginDialog->show();

    // Connect the signal from the login dialog to switch to the register dialog
    connect(loginDialog, &LoginDialog::switchToRegister, this, &MainWindow::showRegisterDialog);

     // Connect the signal from the login dialog to switch to the reset dialog
    connect(loginDialog, &LoginDialog::switchToResetPassword, this, &MainWindow::showResetPasswordDialog);

    // after login authentication, connect the signal(switch to chatdialog) from the tcpManager to the slot
    connect(TcpManager::GetInstance().get(), &TcpManager::signal_switchto_chatdialog, this, &MainWindow::showChatDialog);

    // set windows flags
    loginDialog->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    // !Test: emit the signal_switchto_chatdialog
    emit TcpManager::GetInstance()->signal_switchto_chatdialog();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showRegisterDialog()
{
    registerDialog = new RegisterDialog(this);
    registerDialog->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    // Connect the signal from the register dialog to switch to the login dialog
    connect(registerDialog, &RegisterDialog::signal_switchto_login, this, &MainWindow::showLoginDialog);

    setCentralWidget(registerDialog); // will destroy the login widget

    registerDialog->show();
}

void MainWindow::showLoginDialog()
{

    loginDialog = new LoginDialog(this);
    loginDialog->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    setCentralWidget(loginDialog); // will destroy the register widget
    loginDialog->show();
    // continue Connect the signal from the login dialog to switch to the register dialog
    connect(loginDialog, &LoginDialog::switchToRegister, this, &MainWindow::showRegisterDialog);
    connect(loginDialog, &LoginDialog::switchToResetPassword, this, &MainWindow::showResetPasswordDialog);
}

void MainWindow::showResetPasswordDialog()
{
    resetPasswordDialog = new ResetPasswordDialog(this);
    resetPasswordDialog->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(resetPasswordDialog); // will destroy the login widget

    resetPasswordDialog->show();
    // Connect the signal from the reset password dialog to switch to the login dialog
    connect(resetPasswordDialog, &ResetPasswordDialog::signal_switchto_login, this, &MainWindow::showLoginDialog);
}

void MainWindow::showChatDialog()
{
    chatDialog = new ChatDialog(this);
    chatDialog->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(chatDialog); // will destroy the login widget
    chatDialog->show();
    this->resize(QSize(800, 600));
    this->setMinimumSize(QSize(400, 300));
    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    qDebug() << "Chat dialog shown.";
}
