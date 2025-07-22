#include "customized_label.h"
#include "global.h"
Customized_Label::Customized_Label(QWidget *parent) :
    QLabel(parent),
    _normalState(""),
    _hoverState(""),
    _pressedState(""),
    _isSelected(false)
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

bool Customized_Label::getState() const
{
    return _isSelected; // Return the selection state
}

void Customized_Label::setSelected(bool selected)
{
    _isSelected = selected;
    if (_isSelected) {
        setProperty("state", _pressedState);
    } else {
        setProperty("state", _normalState);
    }
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
    _isSelected = !_isSelected; // Toggle selection state
    setProperty("state", _normalState);
    emit clicked(); // Emit clicked signal
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
