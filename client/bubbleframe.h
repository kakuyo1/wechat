#ifndef BUBBLEFRAME_H
#define BUBBLEFRAME_H
#include <QFrame>
#include <QWidget>
#include <QPainter>
#include "global.h"
#include <QVBoxLayout>
constexpr int BUBBLE_TRIANGLE_LENGTH = 8; // 气泡三角形的长度

class BubbleFrame : public QFrame
{
    Q_OBJECT
public:
    explicit BubbleFrame(MessageRole role, QWidget* parent = nullptr);
    ~BubbleFrame() = default;
    void setBubbleWidget(QWidget* bubbleWidget);
    void setMargin(int margin);
protected:
    virtual void paintEvent(QPaintEvent *event) override;
private:
    MessageRole _messageRole;
    QVBoxLayout* _mainLayout;
    int _margin;
};

#endif // BUBBLEFRAME_H
