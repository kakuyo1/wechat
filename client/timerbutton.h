#ifndef TIMERBUTTON_H
#define TIMERBUTTON_H
#include <QPushButton>
#include <QTimer>
#include <QMouseEvent>

/*********************************************************************
    @brief TimerButton is a QPushButton that can be used to display a countdown timer.
    It inherits from QPushButton and uses a QTimer to manage the countdown.
*********************************************************************/
class TimerButton : public QPushButton
{
    Q_OBJECT
public:
    TimerButton(QWidget* parent = nullptr, int timeout = 10, const QString& text = "获取");
    ~TimerButton();
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    void Init(int timeout, const QString& text); // customize the countdown
private:
    QTimer* timer;
    int timeout; // countdown time in seconds
};

#endif // TIMERBUTTON_H
