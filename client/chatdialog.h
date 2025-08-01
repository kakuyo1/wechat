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

constexpr int MAX_CONTACTS_PER_PAGE = 10; // 每页最多显示的联系人数量

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
    void initializeContactList(); // 收到服务器返回的联系人列表后初始化联系人列表
    void loadMoreContacts(); // 加载更多联系人
protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
private:
    Ui::ChatDialog *ui;
    ChatUIMode chatUIMode;
    bool isLoading; // 是否正在加载会话/联系人列表;
    QList<StateWidget*> sideBarButtons; // 侧边栏按钮列表
    int currentContactIndex; // 当前已加载的联系人数
    std::vector<std::shared_ptr<AuthResponse>> fullContactList; // 全部联系人数据缓存
private slots:
    void slot_load_more_sessionitems();
    void slot_load_more_contactitems();
};

#endif // CHATDIALOG_H
