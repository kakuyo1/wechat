#ifndef TEXTFRAME_H
#define TEXTFRAME_H
#include "bubbleframe.h"
#include <QTextEdit>
#include <QStyleOption>
#include <QEvent>
#include <QTextBlock>
#include <QTextDocument>
/*
    拉伸窗口导致文字排版变化的原理：
    QTextEdit 内部的 QTextDocument 会自动重新排版，
    重新计算每一段的 boundingRect()，导致文字换行变化
*/

class TextFrame : public BubbleFrame
{
    Q_OBJECT
public:
    explicit TextFrame(MessageRole role, const QString& text, QWidget* parent = nullptr);
    ~TextFrame() = default;
protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override; // 事件过滤器，监听窗口拉伸事件
private:
    void initStyleSheet();
    void setMaximumBubbleWidth(const QString& text); // 根据文本内容设置最大气泡宽度
    void adjustTextHeight(); // 每次进行窗口拉伸时调整文本高度()
private:
    QTextEdit* _textEdit; // 文本编辑器
};

#endif // TEXTFRAME_H
