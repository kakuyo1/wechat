#include "friendrequestlist.h"

FriendRequestList::FriendRequestList(QWidget *parent) : QListWidget(parent)
{
    // 设置滚动条初始状态
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 事件过滤器
    this->viewport()->installEventFilter(this);
}

bool FriendRequestList::eventFilter(QObject *watched, QEvent *event)
{
    // 处理鼠标进入离开滚动条的显示与隐藏
    if (watched == this->viewport() && event->type() == QEvent::Enter) {
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    } else if (watched == this->viewport() && event->type() == QEvent::Leave) {
        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    // 处理鼠标滚轮事件
    if (watched == this->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        int steps = wheelEvent->angleDelta().y() / 120; // 每120个单位为一格
        verticalScrollBar()->setValue(verticalScrollBar()->value() - steps * verticalScrollBar()->singleStep());
        return true; // 阻止事件进一步传播
    }
    return QListWidget::eventFilter(watched, event);
}

