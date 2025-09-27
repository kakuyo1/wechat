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
    void updateSessionChatViewMsg(std::vector<std::shared_ptr<TextChatData>> messageBatch); // 更新当前会话的聊天视图消息
    void setSelectedSessionItem(int index); // 设置选中的会话项
    void setSelectedChatPage(int index); // 设置选中的聊天页面
    void intialzeSessionListAndSelectFirstItem(); // 初始化会话列表并选中第一个会话项
protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
private:
    Ui::ChatDialog *ui;
    ChatUIMode chatUIMode;
    bool isLoading; // 是否正在加载会话/联系人列表;
    QList<StateWidget*> sideBarButtons; // 侧边栏按钮列表
    int currentContactIndex; // 当前已加载的联系人数
    std::vector<std::shared_ptr<AuthResponse>> fullContactList; // 全部联系人数据缓存
    int _current_sessionItem; // 当前会话项索引
    QMap<int, QListWidgetItem*> _sessionItemMap; // 会话项索引和列表项的映射
private slots:
    void slot_load_more_sessionitems();
    void slot_load_more_contactitems();
    void slot_switchTo_contactInfoPage();
    void slot_append_chat_message(std::shared_ptr<TextChatData> message); // 用于接收聊天消息并添加到聊天区域
    void slot_get_new_text_chat_batch(std::shared_ptr<TextChatBatch> batch); // 接收新的聊天消息批次
    void slot_sessionItem_clicked(QListWidgetItem* item); // 会话项点击事件处理
    void slot_switchTo_sessionItem_from_contactInfo(std::shared_ptr<AuthResponse> contactInfo); // 从联系人信息页面跳转到对应的会话项
};

#endif // CHATDIALOG_H
