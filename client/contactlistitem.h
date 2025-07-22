#ifndef CONTACTLISTITEM_H
#define CONTACTLISTITEM_H

#include <QWidget>
#include "global.h"
#include "baselistitem.h"
#include <memory>
#include "contactinfodata.h"

namespace Ui {
class ContactListItem;
}

class ContactListItem : public BaseListItem
{
    Q_OBJECT

public:
    explicit ContactListItem(QWidget *parent = nullptr);
    ~ContactListItem() = default;
    virtual QSize sizeHint() const override;
    void setInfo(int uid, QString name, QString avatarUrl);
    void showRedPoint(bool show = false);
private:
    Ui::ContactListItem *ui;
    std::shared_ptr<UserInfo> _userInfo;
};

#endif // CONTACTLISTITEM_H
