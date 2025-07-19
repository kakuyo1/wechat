#include "baselistitem.h"

BaseListItem::BaseListItem(QWidget *parent) :
    QWidget(parent),
    listItemType(ListItemType::NonClickableItem)
{

}

void BaseListItem::setListItem(ListItemType type)
{
    listItemType = type;
}

ListItemType BaseListItem::getListItemType() const
{
    return listItemType;
}

void BaseListItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
