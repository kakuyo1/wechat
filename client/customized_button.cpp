#include "customized_button.h"
#include "global.h"
Customized_button::Customized_button(QWidget *parent) :QPushButton(parent),
    _normalState(""),
    _hoverState(""),
    _pressedState("")
{

}

void Customized_button::setState(const QString& normal, const QString& hover, const QString& pressed)
{
    _normalState = normal;
    _hoverState = hover;
    _pressedState = pressed;
    setProperty("state", normal);
    repolish(this);
    update();
}

void Customized_button::mousePressEvent(QMouseEvent *event)
{
    if (_pressedState.isEmpty()) {
        return;
    }
    setProperty("state", _pressedState);
    repolish(this);
    update();
    QPushButton::mousePressEvent(event);
}

void Customized_button::mouseReleaseEvent(QMouseEvent *event)
{
    if (_normalState.isEmpty()) {
        return;
    }
    setProperty("state", _normalState);
    repolish(this);
    update();
    QPushButton::mouseReleaseEvent(event);
}

void Customized_button::enterEvent(QEnterEvent *event)
{
    if (_hoverState.isEmpty()) {
        return;
    }
    setProperty("state", _hoverState);
    repolish(this);
    update();
    QPushButton::enterEvent(event);
}

void Customized_button::leaveEvent(QEvent *event)
{
    if (_normalState.isEmpty()) {
        return;
    }
    setProperty("state", _normalState);
    repolish(this);
    update();
    QPushButton::leaveEvent(event);
}
