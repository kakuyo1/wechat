#ifndef SESSIONLIST_H
#define SESSIONLIST_H
#include <QListWidget>
#include <QWidget>
#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include "global.h"
/*
    该类用于会话列表的显示和管理。
*/

class SessionList : public QListWidget
{
    Q_OBJECT
public:
    SessionList(QWidget* parent = nullptr);
    ~SessionList() = default;
protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
private:

signals:
    void signal_loading_sessionItems();
};

#endif // SESSIONLIST_H
