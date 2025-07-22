#ifndef FRIENDREQUESTLIST_H
#define FRIENDREQUESTLIST_H
#include <QWidget>
#include <QListWidget>
#include "friendrequestlistitem.h"
#include "global.h"

class FriendRequestList : public QListWidget
{
    Q_OBJECT
public:
    explicit FriendRequestList(QWidget* parent = nullptr);
    ~FriendRequestList() = default;
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // FRIENDREQUESTLIST_H
