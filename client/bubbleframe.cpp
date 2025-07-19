#include "bubbleframe.h"

/*                                                           -----11----
----------------------------------------------------------
|                            |3                             |               |
|     |---------------------------------------- |               |
|     |                                                      |----8--     |
|     |                                                      |----         |
|--3|               TextEdit(Picture)           |    /     3--|
|     |                                                      |  /            |
|     |                                                      |/              |
|     |---------------------------------------- |               |
|                            |3                             |               |
----------------------------------------------------------|
*/


BubbleFrame::BubbleFrame(MessageRole role, QWidget *parent) :
    QFrame(parent),
    _messageRole(role),
    _mainLayout(new QVBoxLayout(this)),
    _margin(3)
{
    if (role == MessageRole::SELF) {
        _mainLayout->setContentsMargins(_margin, _margin, _margin + BUBBLE_TRIANGLE_LENGTH, _margin); // 自己的消息，右边距多出三角形长度
    } else {
        _mainLayout->setContentsMargins(_margin + BUBBLE_TRIANGLE_LENGTH, _margin, _margin, _margin); // 他人的消息，左边距多出三角形长度
    }
    this->setLayout(_mainLayout);
}

void BubbleFrame::setBubbleWidget(QWidget *bubbleWidget)
{
    if (_mainLayout->count() > 0) { // 已经有消息气泡了
        auto * oldWidget = _mainLayout->itemAt(0)->widget();
        if (oldWidget) {
            _mainLayout->replaceWidget(oldWidget, bubbleWidget); // 替换旧的消息气泡
            delete oldWidget; // 删除旧的消息气泡
        }
    } else { // 没有消息气泡
        _mainLayout->addWidget(bubbleWidget); // 添加新的消息气泡
    }
}

void BubbleFrame::setMargin(int margin)
{
    _margin = margin;
}

void BubbleFrame::paintEvent(QPaintEvent *event)
{
    qDebug() << "BubbleFrame::paintEvent()" << this->size();
    QPainter painter(this);
    painter.setPen(Qt::NoPen); // 设置无边框

    if (_messageRole == MessageRole::SELF) {
        // 准备画刷
        QColor bubbleColor(158, 234, 106);
        painter.setBrush(bubbleColor);
        // 绘制气泡方框
        QRect bubbleRect(0, 0, this->width() - BUBBLE_TRIANGLE_LENGTH, this->height());
        painter.drawRoundedRect(bubbleRect, 5, 5);
        // 绘制气泡三角(点位置TODE)
        QPointF points[3] = {
            QPointF(bubbleRect.right(), 12), // 左上角
            QPointF(bubbleRect.right() + BUBBLE_TRIANGLE_LENGTH, 14), // 右侧
            QPointF(bubbleRect.right(), 22) // 左下角
        };
        painter.drawPolygon(points, 3); // 绘制三角形
    } else {
        QColor bubbleColor(Qt::white);
        painter.setBrush(bubbleColor);
        // 绘制气泡方框
        QRect bubbleRect(BUBBLE_TRIANGLE_LENGTH, 0, this->width() - BUBBLE_TRIANGLE_LENGTH, this->height());
        painter.drawRoundedRect(bubbleRect, 5, 5);
        // 绘制气泡三角(点位置TODE)
        QPointF points[3] = {
            QPointF(bubbleRect.x(), 12), // 右上
            QPointF(bubbleRect.x(), 20), // 右下
            QPointF(bubbleRect.x() - BUBBLE_TRIANGLE_LENGTH, 14), // 左侧
        };
        painter.drawPolygon(points, 3); // 绘制三角形
    }

    return QFrame::paintEvent(event); // 调用父类的绘制事件
}
