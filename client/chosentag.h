#ifndef CHOSENTAG_H
#define CHOSENTAG_H

#include <QWidget>
#include <QFrame>
#include <QString>
/*
    该类用于显示已选择的标签，通常用于申请添加好友时的标签显示(如：同学，家人，菜鸟教程，python等等相关标签 。
*/

namespace Ui {
class ChosenTag;
}

class ChosenTag : public QFrame
{
    Q_OBJECT

public:
    explicit ChosenTag(QWidget *parent = nullptr);
    ~ChosenTag() = default;
    void setText(const QString &text);
    int getTextWidth() const;
    int getTextHeight() const;
    QString getText() const;

private:
    Ui::ChosenTag *ui;
    QString _text; // 存储标签文本
    int _textWidth; // 存储标签文本宽度
    int _textHeight; // 存储标签文本高度
private slots:
    void slot_removeTag_clicked();
signals:
    void signal_removeTag(const QString &text); // 信号，用于移除标签
};

#endif // CHOSENTAG_H
