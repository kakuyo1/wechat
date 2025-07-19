#include "statewidget.h"
#include "global.h"

StateWidget::StateWidget(QWidget *parent) :
    QWidget(parent),
    _normalState(""),
    _hoverState(""),
    _pressedState("")
{

}

void StateWidget::setState(const QString& normal, const QString& hover, const QString& pressed)
{
    _normalState = normal;
    _hoverState = hover;
    _pressedState = pressed;
    setProperty("state", normal);
    repolish(this);
    update();
}

void StateWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    return;
}

void StateWidget::mousePressEvent(QMouseEvent *event)
{
    if (_pressedState.isEmpty()) {
        return;
    }
    setProperty("state", _pressedState);
    repolish(this);
    update();
    emit clicked(); // 发出点击信号
    qDebug() << "StateWidget clicked";
    QWidget::mousePressEvent(event);
}

void StateWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (_normalState.isEmpty()) {
        return;
    }
    setProperty("state", _normalState);
    repolish(this);
    update();
    qDebug() << "StateWidget mouseReleaseEvent";
    QWidget::mouseReleaseEvent(event);
}

void StateWidget::enterEvent(QEnterEvent *event)
{
    if (_hoverState.isEmpty()) {
        return;
    }
    setProperty("state", _hoverState);
    repolish(this);
    update();
    qDebug() << "StateWidget enterEvent";
    QWidget::enterEvent(event);
}

void StateWidget::leaveEvent(QEvent *event)
{
    if (_normalState.isEmpty()) {
        return;
    }
    setProperty("state", _normalState);
    repolish(this);
    update();
    qDebug() << "StateWidget leaveEvent";
    QWidget::leaveEvent(event);
}

