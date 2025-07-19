#ifndef STATEWIDGET_H
#define STATEWIDGET_H
#include <QWidget>
#include <QStyleOption>
#include <QPainter>
/*
    注意：这个类是一个用于侧边栏状态显示的widget，比如三态normal，hover和pressed状态下的图片的切换。
    右上角红点的显示...
*/

class StateWidget : public QWidget
{
    Q_OBJECT
public:
    StateWidget(QWidget* parent = nullptr);
    ~StateWidget() = default;
    void setState(const QString& normal, const QString& hover, const QString& pressed);
protected:
    void paintEvent(QPaintEvent* event); // 重写绘制事件
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEnterEvent *event) override; // normal to hover
    virtual void leaveEvent(QEvent *event) override; // hover to normal
private:
    QString _normalState;
    QString _hoverState;
    QString _pressedState;
signals:
    void clicked();
};

#endif // STATEWIDGET_H
