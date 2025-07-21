#include "statewidget.h"
#include "global.h"

StateWidget::StateWidget(QWidget *parent) :
    QWidget(parent),
    _normalState(""),
    _hoverState(""),
    _pressedState(""),
    _redPointLabel(new QLabel(this)),
    _currentState(StateType::Normal)
{
    setCursor(Qt::PointingHandCursor);
    addRedPoint();
}

void StateWidget::setState(const QString& normal, const QString& hover, const QString& pressed)
{
    _normalState = normal;
    _hoverState = hover;
    _pressedState = pressed;
    setProperty("state", normal);
    repolish(this);
    update();
}

void StateWidget::clearState()
{
    showRedPoint(false);
    _currentState = StateType::Normal;
    setProperty("state", _normalState);
    repolish(this);
    update();
}

void StateWidget::addRedPoint()
{
    QPixmap redPointPixmap(":/images/red_point.png");
    QVBoxLayout* layout = new QVBoxLayout(this);
    _redPointLabel->setObjectName("redPoint");
    _redPointLabel->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(_redPointLabel);
    redPointPixmap = redPointPixmap.scaled(_redPointLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _redPointLabel->setScaledContents(true);
    _redPointLabel->setPixmap(redPointPixmap);
    setLayout(layout);
    _redPointLabel->setVisible(false); // 默认隐藏红点
}

void StateWidget::showRedPoint(bool show)
{
    _redPointLabel->setVisible(show);
}

StateType StateWidget::currentState() const
{
    if (property("state") == _normalState) {
        return StateType::Normal;
    } else if (property("state") == _hoverState) {
        return StateType::Hover;
    } else if (property("state") == _pressedState) {
        return StateType::Pressed;
    }
    return StateType::Normal; // 默认返回Normal状态
}

void StateWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    return;
}

void StateWidget::mousePressEvent(QMouseEvent *event)
{
    if (_pressedState.isEmpty()) {
        return;
    }
    setProperty("state", _pressedState);
    repolish(this);
    update();
    emit clicked(); // 发出点击信号
    qDebug() << "StateWidget clicked";
    QWidget::mousePressEvent(event);
}

void StateWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (_normalState.isEmpty()) {
        return;
    }
    setProperty("state", _normalState);
    repolish(this);
    update();
    qDebug() << "StateWidget mouseReleaseEvent";
    QWidget::mouseReleaseEvent(event);
}

void StateWidget::enterEvent(QEnterEvent *event)
{
    if (_hoverState.isEmpty()) {
        return;
    }
    setProperty("state", _hoverState);
    repolish(this);
    update();
    qDebug() << "StateWidget enterEvent";
    QWidget::enterEvent(event);
}

void StateWidget::leaveEvent(QEvent *event)
{
    if (_normalState.isEmpty()) {
        return;
    }
    setProperty("state", _normalState);
    repolish(this);
    update();
    qDebug() << "StateWidget leaveEvent";
    QWidget::leaveEvent(event);
}

