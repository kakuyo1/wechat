#include "customized_label.h"
#include "global.h"
Customized_Label::Customized_Label(QWidget *parent) :
    QLabel(parent),
    _normalState(""),
    _hoverState(""),
    _pressedState("")
{

}

void Customized_Label::setState(const QString &normal, const QString &hover, const QString &pressed)
{
    _normalState = normal;
    _hoverState = hover;
    _pressedState = pressed;
    setProperty("state", normal);
    repolish(this);
    update();
}

void Customized_Label::mousePressEvent(QMouseEvent *event)
{
    if (_pressedState.isEmpty()) {
        return;
    }
    setProperty("state", _pressedState);
    repolish(this);
    update();
    QLabel::mousePressEvent(event);
}

void Customized_Label::mouseReleaseEvent(QMouseEvent *event)
{
    if (_normalState.isEmpty()) {
        return;
    }
    setProperty("state", _normalState);
    repolish(this);
    update();
    QLabel::mouseReleaseEvent(event);
}

void Customized_Label::enterEvent(QEnterEvent *event)
{
    if (_hoverState.isEmpty()) {
        return;
    }
    setProperty("state", _hoverState);
    repolish(this);
    update();
    QLabel::enterEvent(event);
}

void Customized_Label::leaveEvent(QEvent *event)
{
    if (_normalState.isEmpty()) {
        return;
    }
    setProperty("state", _normalState);
    repolish(this);
    update();
    QLabel::leaveEvent(event);
}
