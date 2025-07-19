#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "global.h"
#include <QRandomGenerator>
#include <QListWidgetItem>
#include <QMovie>
#include <QTimer>
namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();
private:
    void showSearchList(bool show);
    void Test_addSessionItem();
    virtual QSize sizeHint() const override;
private:
    Ui::ChatDialog *ui;
    ChatUIMode chatUIMode;
    bool isLoading; // 是否正在加载会话列表;
private slots:
    void slot_load_more_sessionitems();
};

#endif // CHATDIALOG_H
