#ifndef CONTACTGROUPTIPITEM_H
#define CONTACTGROUPTIPITEM_H
#include <QWidget>
#include "global.h"
#include "baselistitem.h"

namespace Ui {
class ContactGroupTipItem;
}

class ContactGroupTipItem : public BaseListItem
{
    Q_OBJECT

public:
    explicit ContactGroupTipItem(QWidget *parent = nullptr);
    ~ContactGroupTipItem()= default;
    virtual QSize sizeHint() const override;
    void setGroupTipText(const QString& text);
private:

private:
    QString _groupTipText; // 组提示文本
    Ui::ContactGroupTipItem *ui;
};

#endif // CONTACTGROUPTIPITEM_H
