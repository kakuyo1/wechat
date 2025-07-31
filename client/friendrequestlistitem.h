#ifndef FRIENDREQUESTLISTITEM_H
#define FRIENDREQUESTLISTITEM_H

#include <QWidget>
#include "baselistitem.h"
#include <memory>
#include "contactinfodata.h"

namespace Ui {
class FriendRequestListItem;
}

class FriendRequestListItem : public BaseListItem
{
    Q_OBJECT

public:
    explicit FriendRequestListItem(QWidget *parent = nullptr);
    ~FriendRequestListItem() = default;
    virtual QSize sizeHint() const override;
    void showAddButton(bool show = false);
    void setInfo(std::shared_ptr<RequestInfo> requestInfo);// (Test)
    void setInfoByAddContactResponse(std::shared_ptr<AddContactResponse> addContactResponse); // from server
private:
    Ui::FriendRequestListItem *ui;
    std::shared_ptr<RequestInfo> _requestInfo; // 好友请求信息(Test)
    std::shared_ptr<AddContactResponse> _addContactItemInfo; // 好友申请信息
    bool _AlreadyAdded; // 是否已经添加好友
signals:
    void signal_addFriendClicked(std::shared_ptr<RequestInfo> requestInfo); // 添加好友按钮点击信号;
};

#endif // FRIENDREQUESTLISTITEM_H
