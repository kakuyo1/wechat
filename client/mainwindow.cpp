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

    // set windows flags
    loginDialog->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
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
    loginDialog->hide();
    registerDialog->show();
}

void MainWindow::showLoginDialog()
{
    loginDialog = new LoginDialog(this);
    loginDialog->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    setCentralWidget(loginDialog); // will destroy the register widget
    registerDialog->hide();
    loginDialog->show();
    // continue Connect the signal from the login dialog to switch to the register dialog
    connect(loginDialog, &LoginDialog::switchToRegister, this, &MainWindow::showRegisterDialog);
}
