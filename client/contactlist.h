#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QWidget>
#include <QListWidget>
#include "contactlistitem.h"
#include "contactgrouptipitem.h"
#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QRandomGenerator>
class ContactList : public QListWidget
{
    Q_OBJECT
public:
    explicit ContactList(QWidget* parent = nullptr);
    ~ContactList() = default;
    void showAddContactRedPoint(bool show = false);
protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
private:
    void Test_AddContacts(); // 测试添加联系人
private slots:
    void slot_contactItem_clicked(QListWidgetItem* item); // 联系人项点击事件, 弹出联系人信息
    void slot_AfterACK_addNewContactItem(std::shared_ptr<AuthResponse>); // 处理添加新联系人项的ACK响应
private:
    ContactListItem* _addContactItem; // "新的朋友"
    QListWidgetItem* contactGroupListItem; // 用于显示联系人组的提示项
signals:
    void signal_load_contacts(); // 信号，用于加载联系人列表
    void signal_switchTo_contactInfoPage(); // 响应slot_contactItem_clicked切换到联系人信息页面
    void signal_switchTo_friendRequestPage(); // 响应slot_contactItem_clicked切换到好友申请信息页面
};

#endif // CONTACTLIST_H
