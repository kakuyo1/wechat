#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , loginDialog(new LoginDialog())
    , registerDialog(new RegisterDialog())
{
    ui->setupUi(this);
    // Initialize the login dialog and set it as the central widget
    setCentralWidget(loginDialog);
    loginDialog->show();

    // Connect the signal from the login dialog to switch to the register dialog
    connect(loginDialog, &LoginDialog::switchToRegister, this, &MainWindow::showRegisterDialog);

    // set windows flags
    loginDialog->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    registerDialog->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showRegisterDialog()
{
    setCentralWidget(registerDialog);
    loginDialog->hide();
    registerDialog->show();
}
