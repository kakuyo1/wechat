#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H
#include <QLabel>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QPixmap>

/*
    这个类是一个可点击的标签，仅用于显示密码的隐藏和显示状态。
    当鼠标点击时，它会切换显示和隐藏密码的图标。
    鼠标悬停时，图标会变为悬停状态。
    鼠标离开时，图标会变为正常状态。
    注意：这个类不处理密码的实际隐藏或显示逻辑，仅用于图标的切换。
*/

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    ClickableLabel(QWidget* parent = nullptr);
    ~ClickableLabel() = default;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEnterEvent *event) override; // normal to hover
    virtual void leaveEvent(QEvent *event) override; // hover to normal
    void updatePixmap();
    bool getState() const;
private:
    bool isClicked;
    QPixmap hidePasswordPixmap;
    QPixmap hidePasswordHoverPixmap;
    QPixmap showPasswordPixmap;
    QPixmap showPasswordHoverPixmap;
signals:
    void clicked();
};

#endif // CLICKABLELABEL_H
