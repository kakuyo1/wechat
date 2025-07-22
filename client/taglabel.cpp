#include "taglabel.h"

TagLabel::TagLabel(QWidget *parent) : QLabel(parent)
{
    setCursor(Qt::PointingHandCursor);
}

void TagLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked(); // emit the clicked signal
        return;
    }
    QLabel::mouseReleaseEvent(event); // call base class implementation
}
