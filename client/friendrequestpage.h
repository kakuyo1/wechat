#ifndef FRIENDREQUESTPAGE_H
#define FRIENDREQUESTPAGE_H

#include <QWidget>
#include "contactinfodata.h"
#include <memory>
#include <unordered_map>
#include "friendrequestlistitem.h"
#include <QStyleOption>
#include <QPainter>
#include "tcpmanager.h"
#include <QRandomGenerator>
#include "friendauthdialog.h"

namespace Ui {
class FriendRequestPage;
}

class FriendRequestPage : public QWidget
{
    Q_OBJECT

public:
    explicit FriendRequestPage(QWidget *parent = nullptr);
    ~FriendRequestPage() = default;
protected:
    virtual void paintEvent(QPaintEvent *event) override;
private:
    void Test_LoadFakeRequestData();
    void intializeRequestList();
private:
    Ui::FriendRequestPage *ui;
    std::unordered_map<int, FriendRequestListItem*> _requestItems;
    std::shared_ptr<FriendAuthDialog> authDialog;
private slots:
    void slot_add_new_contact_request(std::shared_ptr<AddContactResponse> response);
    void slot_addBtn_clicked(); // 同意添加好友，弹出验证页面
    void slot_afterACK_handleTheRequestItem(std::shared_ptr<AuthResponse> response); // 处理好友认证请求的ACK响应
signals:
    void signal_sideContact_showRedPoint();
};

#endif // FRIENDREQUESTPAGE_H
