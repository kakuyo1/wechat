#ifndef ADDCONTACTITEM_H
#define ADDCONTACTITEM_H

#include <QWidget>
#include "baselistitem.h"
#include "global.h"

namespace Ui {
class AddContactItem;
}

class AddContactItem : public BaseListItem
{
    Q_OBJECT

public:
    explicit AddContactItem(QWidget *parent = nullptr);
    ~AddContactItem();
    QSize sizeHint() const override;
private:
    Ui::AddContactItem *ui;
    ListItemType _itemType;
};

#endif // ADDCONTACTITEM_H
