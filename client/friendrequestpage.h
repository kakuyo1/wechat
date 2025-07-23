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

namespace Ui {
class FriendRequestPage;
}

class FriendRequestPage : public QWidget
{
    Q_OBJECT

public:
    explicit FriendRequestPage(QWidget *parent = nullptr);
    ~FriendRequestPage() = default;
    void addNewContactRequest(std::shared_ptr<AddContactRequest> requestInfo);
protected:
    virtual void paintEvent(QPaintEvent *event) override;
private:
    void Test_LoadFakeRequestData();
private:
    Ui::FriendRequestPage *ui;
    std::unordered_map<int, FriendRequestListItem*> _requestItems;
private slots:
    void slot_get_authResponse(std::shared_ptr<AuthResponse> response);
};

#endif // FRIENDREQUESTPAGE_H
