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
    void setInfo(std::shared_ptr<AuthResponse> response);
    void setAddContactItemInfoByLocal(QString name, QString avatarUrl);
    void showRedPoint(bool show = false);
    std::shared_ptr<AuthResponse> getContactInfo() { return _contactInfo; }
private:
    Ui::ContactListItem *ui;
    std::shared_ptr<AuthResponse> _contactInfo; // 联系人信息
};

#endif // CONTACTLISTITEM_H
