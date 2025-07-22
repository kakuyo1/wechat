#include "contactgrouptipitem.h"
#include "ui_contactgrouptipitem.h"

ContactGroupTipItem::ContactGroupTipItem(QWidget *parent)
    : BaseListItem(parent)
    , ui(new Ui::ContactGroupTipItem)
{
    ui->setupUi(this);
    setListItem(ListItemType::ContactGroupTipItem);
}


QSize ContactGroupTipItem::sizeHint() const
{
    return QSize(250, 25);
}

void ContactGroupTipItem::setGroupTipText(const QString &text)
{
    ui->contactGroupTip_label->setText(text);
}
