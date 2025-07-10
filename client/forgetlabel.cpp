#include "forgetlabel.h"

ForgetLabel::ForgetLabel(QWidget *parent) : QLabel(parent)
{

}

void ForgetLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
    QLabel::mousePressEvent(event);
}


