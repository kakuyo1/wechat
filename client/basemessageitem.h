#ifndef BASEMESSAGEITEM_H
#define BASEMESSAGEITEM_H

#include <QWidget>
#include <QLabel>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QGridLayout>
#include <QPainter>
#include <QStyleOption>
#include "global.h"

/*
 自己的消息
 |             | NameLabel | IconLabel  |
 |--------- |----------------|                    |
 | Spacer| Bubble        |                    |
 |--------- |----------------|-------------- |


 他人的消息
 |IconLabel | NameLabel |             |
 |-------------|----------------|              |
 |                 | Bubble        |Spacer  |
 |-------------|----------------|--------- |
*/

class BaseMessageItem : public QWidget
{
    Q_OBJECT
public:
    explicit BaseMessageItem(MessageRole role, QWidget *parent = nullptr);
    ~BaseMessageItem() = default;
    void setBubbleWidget(QWidget* bubbleWidget);
    void setUserAvatar(const QString& avatarPath);
    void setUserName(const QString& name);
private:
    MessageRole _messageRole;
    QLabel* _avatarLabel; // 用户头像
    QLabel* _nameLabel; // 用户名
    QWidget* _messageBubbleWidget; // 消息内容区域
    QSpacerItem* _spacer; // 用于调整布局
    QGridLayout* _mainLayout; // 主布局
};

#endif // BASEMESSAGEITEM_H
