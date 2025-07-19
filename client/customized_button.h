#ifndef CUSTOMIZED_BUTTON_H
#define CUSTOMIZED_BUTTON_H
#include <QPushButton>
#include <QWidget>

/*
    注意：这个类不同于ClickableLabel，此类是一个自定义按钮，可以
    用于任意按钮的normal，hover和pressed状态下图片的切换。
*/
class Customized_button : public QPushButton
{
    Q_OBJECT
public:
    Customized_button(QWidget* parent = nullptr);
    ~Customized_button() = default;
    void setState(const QString& normal, const QString& hover, const QString& pressed);
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEnterEvent *event) override; // normal to hover
    virtual void leaveEvent(QEvent *event) override; // hover to normal
private:
    QString _normalState;
    QString _hoverState;
    QString _pressedState;
};

#endif // CUSTOMIZED_BUTTON_H
