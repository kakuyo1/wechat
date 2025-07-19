#ifndef BASELISTITEM_H
#define BASELISTITEM_H
#include <QWidget>
#include "global.h"
#include <QPainter>
#include <QStyleOption>
class BaseListItem : public QWidget
{
    Q_OBJECT
public:
    BaseListItem(QWidget* parent = nullptr);
    ~BaseListItem() = default;
    void setListItem(ListItemType type);
    ListItemType getListItemType() const;
protected:
    virtual void paintEvent(QPaintEvent* event) override;
private:
    ListItemType listItemType;
};

#endif // BASELISTITEM_H
