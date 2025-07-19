#ifndef SESSIONLISTITEM_H
#define SESSIONLISTITEM_H
#include <QWidget>
#include "baselistitem.h"

namespace Ui {
class SessionListItem;
}

class SessionListItem : public BaseListItem
{
    Q_OBJECT

public:
    explicit SessionListItem(QWidget *parent = nullptr);
    ~SessionListItem();
    QSize sizeHint() const override;
    void setSessionInfo(const QString &sessionName, const QString & iconPath, const QString &message);
private:
    Ui::SessionListItem *ui;
    QString _sessionName;
    QString _iconPath;
    QString _messageContent;
};

#endif // SESSIONLISTITEM_H
