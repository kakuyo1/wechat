#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget *parent) : QLabel(parent), isClicked(false)
{
    hidePasswordPixmap = QPixmap(":/images/hidePassword_normal.png");
    hidePasswordHoverPixmap = QPixmap(":/images/hidePassword_hover.png");
    showPasswordPixmap = QPixmap(":/images/showPassword_normal.png");
    showPasswordHoverPixmap = QPixmap(":/images/showPassword_hover.png");

    if (hidePasswordPixmap.isNull() || hidePasswordHoverPixmap.isNull() ||
        showPasswordPixmap.isNull() || showPasswordHoverPixmap.isNull()) {
        qWarning("ClickableLabel: Pixmaps not loaded correctly.");
    }
    setScaledContents(true);
    setPixmap(hidePasswordPixmap); // default
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        updatePixmap();
    }
    QLabel::mouseReleaseEvent(event);
}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isClicked = !isClicked; // toggle the state
        updatePixmap();
        emit clicked();
    }
    QLabel::mousePressEvent(event);
}

void ClickableLabel::enterEvent(QEnterEvent *event)
{
    updatePixmap();
    QLabel::enterEvent(event);
}

void ClickableLabel::leaveEvent(QEvent *event)
{
    updatePixmap();
    QLabel::leaveEvent(event);
}

void ClickableLabel::updatePixmap()
{
    if (isClicked) {
        if (underMouse()) {
            setPixmap(showPasswordHoverPixmap);
        } else {
            setPixmap(showPasswordPixmap);
        }
    } else {
        if (underMouse()) {
            setPixmap(hidePasswordHoverPixmap);
        } else {
            setPixmap(hidePasswordPixmap);
        }
    }
}

bool ClickableLabel::getState() const
{
    return isClicked;
}
