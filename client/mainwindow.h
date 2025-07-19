#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"
#include "resetpassworddialog.h"
#include "chatdialog.h"

/************************************************************
 *  @file   mainwindow.h
 *  @brief  Header file for the MainWindow class
 *  @author YeMeng
 *  @date   2025-07-06
 * ***********************************************************/

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LoginDialog *loginDialog;
    RegisterDialog *registerDialog;
    ResetPasswordDialog *resetPasswordDialog;
    ChatDialog *chatDialog;

private slots:
    void showRegisterDialog();
    void showLoginDialog();
    void showResetPasswordDialog();
    void showChatDialog();
};
#endif // MAINWINDOW_H
