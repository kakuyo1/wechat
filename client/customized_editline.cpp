#include "customized_editline.h"

Customized_EditLine::Customized_EditLine(QWidget *parent) :
    QLineEdit(parent),
    placeholderText("搜索"),
    maxTextLength(15),
    searchIconPath(":/images/search.png"),
    clearIconPath(":/images/clear.png")
{
    MsetPlaceholderText(placeholderText);
    MsetMaxTextLength(15);
    equipWithSearchAction();

    // 设置输入限制
    connect(this, &Customized_EditLine::textChanged, this, &Customized_EditLine::limitInputLength);
    // 设置清除按钮
    connect(this, &QLineEdit::textChanged, this, &Customized_EditLine::equipWithClearButton);
}

void Customized_EditLine::focusOutEvent(QFocusEvent *event)
{
    emit signal_focusOut(); // 发出信号，通知外部处理焦点丢失事件
    QLineEdit::focusOutEvent(event); // 调用基类的焦点离开事件处理
}

void Customized_EditLine::focusInEvent(QFocusEvent *event)
{
    emit signal_focusIn(); // 发出信号，通知外部处理焦点获取事件
    QLineEdit::focusInEvent(event); // 调用基类的焦点进入事件处理
}

void Customized_EditLine::MsetPlaceholderText(const QString &text)
{
    placeholderText = text;
    this->setPlaceholderText(placeholderText);
}

void Customized_EditLine::MsetMaxTextLength(int maxLength)
{
    maxTextLength = maxLength;
}

void Customized_EditLine::limitInputLength(const QString &text)
{
    if (text.length() > maxTextLength) {
        QString limited = text.left(maxTextLength);  // 用临时变量保存截断后的字符串
        QLineEdit::setText(limited);                 // 更新文本
    }
}

void Customized_EditLine::equipWithClearButton()
{
    // 当输入框有文字时出现靠右的清除按钮
    if (this->text().isEmpty()) {
        this->setClearButtonEnabled(false); // 禁用清除按钮
    } else {
        this->setClearButtonEnabled(true); // 启用清除按钮
    }
}

void Customized_EditLine::equipWithSearchAction()
{
    // 添加搜索图标到输入框左侧
    QAction *searchAction = new QAction(QIcon(searchIconPath),"", this);
    this->addAction(searchAction, QLineEdit::LeadingPosition); // 添加到左侧
}
