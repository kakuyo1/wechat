#include "sessionlist.h"

SessionList::SessionList(QWidget *parent) :
    QListWidget(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->viewport()->installEventFilter(this); // 安装事件过滤器
}

bool SessionList::eventFilter(QObject *watched, QEvent *event)
{
    // 检测是否鼠标的进入和离开viewport事件
    if (watched == this->viewport()) {
        if (event->type() == QEvent::Enter) {
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else if (event->type() == QEvent::Leave) {
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    // 检测鼠标滚轮事件
    if (watched == this->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        int numberOfSteps = wheelEvent->angleDelta().y() / 120; // 每120个单位为一个步长, 相当于一次滚动
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numberOfSteps); // 滚动条的值减去步长(上滚为正值，下滚为负值)
        // 如果达到底部，发出加载更多会话的信号
        if (this->verticalScrollBar()->value() == this->verticalScrollBar()->maximum()) {
            emit signal_loading_sessionItems(); // 发出加载会话列表的信号
        }
        return true; // 阻止事件继续传播
    }
    return QListWidget::eventFilter(watched, event); // 继续处理其他事件
}

