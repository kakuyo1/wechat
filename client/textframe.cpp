#include "textframe.h"

TextFrame::TextFrame(MessageRole role, const QString &text, QWidget *parent) :
    BubbleFrame(role, parent),
    _textEdit(new QTextEdit(this))
{
    // _textEdit->setLineWrapMode(QTextEdit::NoWrap); // 设置不自动换行
    _textEdit->setReadOnly(true);
    _textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _textEdit->installEventFilter(this);
    QFont font("Microsoft YaHei", 12);
    _textEdit->setFont(font);
    setMaximumBubbleWidth(text);
    setBubbleWidget(_textEdit); // 设置文本编辑器为气泡内容
    initStyleSheet();
}

bool TextFrame::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == _textEdit && (event->type() == QEvent::Resize || event->type() == QEvent::Paint)) {
        adjustTextHeight(); // 调整文本高度
    }
    return BubbleFrame::eventFilter(watched, event); // 继续处理其他事件
}

void TextFrame::initStyleSheet()
{
    _textEdit->setStyleSheet(
        "QTextEdit {"
        " background:transparent;"
        " border:none;"
        " padding-right:8px;"
        "}"
        );
}

void TextFrame::setMaximumBubbleWidth(const QString &text)
{
    _textEdit->setPlainText(text); // 设置文本内容, 以换行符号分隔获取不同的文本块
    //最大气泡宽度 = 文本内容宽度 + TextEdit的左右边距 + 父类BubbleFrame的左右margin

    // 1. 父类BubbleFrame的margin
    int fatherBubbleMarginLeft = this->layout()->contentsMargins().left();
    int fatherBubbleMarginRight = this->layout()->contentsMargins().right();
    int fatherMargin = fatherBubbleMarginLeft + fatherBubbleMarginRight;

    // 2. TextEdit的边距
    qreal documentMarginLeftAndRight = (_textEdit->document()->documentMargin()) * 2;

    // 3. 文本内容宽度
    QFontMetrics fm(_textEdit->font()); // 获取字体度量(因为不同字体可能宽度不同)
    QTextDocument* document = _textEdit->document();
    int max_content_width = 0;
    // 遍历文本内容，计算最大宽度
    for (QTextBlock block = document->begin(); block != document->end(); block = block.next()) { // 以换行符号分隔获取不同的文本块
        QString text = block.text();
        int width = fm.horizontalAdvance(text); // 获取文本宽度
        if (width > max_content_width) {
            max_content_width = width; // 更新最大宽度
        }
    }
    const int maxBubbleWidth = 300;
    const int minBubbleWidth = 80;
    int targetWidth = max_content_width + documentMarginLeftAndRight + fatherMargin;
    if (targetWidth > maxBubbleWidth) {
        targetWidth = maxBubbleWidth;
    }
    if (targetWidth < minBubbleWidth) {
        targetWidth = minBubbleWidth;
    }
    setMaximumWidth(targetWidth);// 根据文本内容设置最大气泡宽度
    _textEdit->setFixedWidth(targetWidth); // 这里固定TextEdit宽度，使其换行在maxBubbleWidth处发生
}

void TextFrame::adjustTextHeight()
{
     //最大气泡高度 = 文本内容高度 + TextEdit的上下边距 + 父类BubbleFrame的上下margin

    // 1. 父类BubbleFrame的margin
    int fatherBubbleMarginTop = this->layout()->contentsMargins().top();
    int fatherBubbleMarginBottom = this->layout()->contentsMargins().bottom();
    int fatherMargin = fatherBubbleMarginTop + fatherBubbleMarginBottom;

    // 2. TextEdit的边距
    qreal documentMarginTopAndBottom = _textEdit->document()->documentMargin() * 2;

    // 3. 文本内容高度
    int total_content_height = 0;
    QTextDocument* document = _textEdit->document();
    for (QTextBlock block = document->begin(); block != document->end(); block = block.next()) { // 以换行符号分隔获取不同的文本块
        total_content_height += block.layout()->boundingRect().height();
    }
    // 设置固定高度(注意是fixed)
    setFixedHeight(total_content_height + documentMarginTopAndBottom + fatherMargin); // 根据文本内容设置固定气泡高度
}
