#include "timerbutton.h"

TimerButton::TimerButton(QWidget* parent, int timeout, const QString& text) :
    QPushButton(text, parent), timeout(timeout)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, timeout, text](){
        this->timeout--;
        if (this->timeout <= 0) {
            this->timeout = timeout; // reset timeout
            this->setText(text);
            this->setEnabled(true);
            timer->stop();
            return;
        }
        this->setText(QString::number(this->timeout));
    });
}

TimerButton::~TimerButton()
{
    timer->stop();
}

void TimerButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->setEnabled(false);
        this->setText(QString::number(timeout));
        timer->start(1000); // start the countdown timer
        emit clicked(); // emit the clicked signal
    }
    QPushButton::mouseReleaseEvent(event); // call base class implementation
}

void TimerButton::Init(int timeout, const QString &text)
{
    this->timeout = timeout;
    this->setText(text);
}

