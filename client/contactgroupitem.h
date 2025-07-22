#ifndef CONTACTGROUPITEM_H
#define CONTACTGROUPITEM_H
#include <QWidget>
#include "global.h"
#include "baselistitem.h"

class ContactGroupItem : public BaseListItem
{
    Q_OBJECT
public:
    explicit ContactGroupItem(QWidget* parent = nullptr);
    ~ContactGroupItem() = default;
private:
    virtual QSize sizeHint() const override;
    void setGroupTipText(const QString& text);
private:
    QString _groupTipText; // 组提示文本
};

#endif // CONTACTGROUPITEM_H
