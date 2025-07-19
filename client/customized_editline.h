#ifndef CUSTOMIZED_EDITLINE_H
#define CUSTOMIZED_EDITLINE_H
#include <QLineEdit>
#include <QWidget>
#include <QAction>
/*
    该类用于搜索框
*/

class Customized_EditLine : public QLineEdit
{
    Q_OBJECT
public:
    Customized_EditLine(QWidget *parent = nullptr);
    ~Customized_EditLine() = default;
protected:
    virtual void focusOutEvent(QFocusEvent *event) override; // 离开搜索列表，进入聊天列表(默认)
    virtual void focusInEvent(QFocusEvent *event) override; // 进入搜索列表，显示搜索内容
private:
    void MsetPlaceholderText(const QString &text);
    void MsetMaxTextLength(int maxLength);
    void limitInputLength(const QString &text); // 限制输入长度，超过最大长度后自动截断
    void equipWithClearButton(); // 输入文字后出现在搜索栏右侧的关闭按钮，点击后清空搜索内容
    void equipWithSearchAction();  // 出现在搜索框左侧，仅用于搜索框的图标, 无实际功能
private:
    QString placeholderText;
    int maxTextLength;
    QString searchIconPath; // 搜索图标路径
    QString clearIconPath;  // 清除图标路径
signals:
    void signal_focusOut();
    void signal_focusIn();
};

#endif // CUSTOMIZED_EDITLINE_H
