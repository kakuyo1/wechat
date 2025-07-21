#include "addcontactitem.h"
#include "ui_addcontactitem.h"

AddContactItem::AddContactItem(QWidget *parent)
    : BaseListItem(parent)
    , ui(new Ui::AddContactItem)
    , _itemType(ListItemType::AddContactTipItem)
{
    ui->setupUi(this);
    setListItem(_itemType); // 设置列表项类型
}

AddContactItem::~AddContactItem()
{
    delete ui;
}

QSize AddContactItem::sizeHint() const
{
    return QSize(250, 70);
}
