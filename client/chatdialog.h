#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include "global.h"
#include <QRandomGenerator>
#include <QListWidgetItem>
#include <QMovie>
#include <QTimer>
#include <statewidget.h>
#include <QList>
#include <QEvent>
#include <QWidget>
#include "contactlist.h"
namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    QString getSearchLineEditText() const;
    ~ChatDialog();
private:
    void showSearchList(bool show);
    void Test_addSessionItem();
    void Test_AddMoreContacts();
    virtual QSize sizeHint() const override;
    void addSideBarButtons(StateWidget* button);
protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
private:
    Ui::ChatDialog *ui;
    ChatUIMode chatUIMode;
    bool isLoading; // 是否正在加载会话/联系人列表;
    QList<StateWidget*> sideBarButtons; // 侧边栏按钮列表
private slots:
    void slot_load_more_sessionitems();
    void slot_load_more_contactitems();
};

#endif // CHATDIALOG_H
