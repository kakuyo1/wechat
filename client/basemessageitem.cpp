#include "basemessageitem.h"

BaseMessageItem::BaseMessageItem(MessageRole role, QWidget *parent) :
    QWidget(parent),
    _messageRole(role),
    _avatarLabel(new QLabel(this)),
    _nameLabel(new QLabel(this)),
    _spacer(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum)),
    _mainLayout(new QGridLayout(this))
{
    _messageBubbleWidget = new QWidget(this); // 初始化消息气泡占位符
    // 设置namelabel样式
    _nameLabel->setObjectName("chat_name_label");
    QFont font = QFont("Microsoft YaHei");
    font.setPointSize(9);
    _nameLabel->setFont(font);
    _nameLabel->setFixedHeight(20);
    // 设置头像标签样式(图片路径由外部设置)
    _avatarLabel->setObjectName("chat_avatar_label");
    _avatarLabel->setFixedSize(42, 42);
    // 设置布局
    _mainLayout->setVerticalSpacing(3);
    _mainLayout->setHorizontalSpacing(3);
    _mainLayout->setContentsMargins(3, 3, 3, 3); // 设置布局边距
    // 判断消息角色，设置布局方向
    if (_messageRole == MessageRole::SELF) {
        _nameLabel->setContentsMargins(0, 0, 8, 0); // 自己的消息，右边距8
        _nameLabel->setAlignment(Qt::AlignRight);
        _mainLayout->addWidget(_nameLabel, 0, 1, 1,1); // 第一行第二列，占用一格
        _mainLayout->addWidget(_avatarLabel, 0, 2, 2, 1, Qt::AlignTop); // 第一行第三列，向行方向占用两格
        _mainLayout->addItem(_spacer, 1, 0, 1, 1); // 第一行第一列，占用一格
        _mainLayout->addWidget(_messageBubbleWidget, 1, 1, 1, 1); // 第二行第二列，占用一格
        //设置spacer列和messageBubbleWidget（name）列的伸缩因子2:3
        _mainLayout->setColumnStretch(0, 2);
        _mainLayout->setColumnStretch(1, 3);
    } else {
        _nameLabel->setContentsMargins(8, 0, 0, 0); // 他人的消息，左边距8
        _nameLabel->setAlignment(Qt::AlignLeft);
        _mainLayout->addWidget(_nameLabel, 0, 1, 1, 1); // 第一行第二列，占用一格
        _mainLayout->addWidget(_avatarLabel, 0, 0, 2, 1, Qt::AlignTop); // 第一行第一列，向行方向占用两格
        _mainLayout->addItem(_spacer, 1, 2, 1, 1); // 第一行第三列，占用一格
        _mainLayout->addWidget(_messageBubbleWidget, 1, 1, 1, 1); // 第二行第二列，占用一格
        //设置spacer列和messageBubbleWidget（name）列的伸缩因子2:3
        _mainLayout->setColumnStretch(1, 3);
        _mainLayout->setColumnStretch(2, 2);
    }
    this->setLayout(_mainLayout); // 设置主布局
}

void BaseMessageItem::setBubbleWidget(QWidget *bubbleWidget)
{
    _mainLayout->replaceWidget(_messageBubbleWidget, bubbleWidget); // 替换旧的消息气泡
    delete _messageBubbleWidget; // 删除旧的消息气泡
    _messageBubbleWidget = bubbleWidget;
}

void BaseMessageItem::setUserAvatar(const QString &avatarPath)
{
    if (!_avatarLabel) {
        _avatarLabel = new QLabel(this);
    }
    QPixmap avatarPixmap(avatarPath);
    if (!avatarPixmap.isNull()) {
        _avatarLabel->setPixmap(avatarPixmap.scaled(_avatarLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        _avatarLabel->setScaledContents(true); // 确保头像适应标签大小
    } else {
        _avatarLabel->clear(); // 如果路径无效，清除头像
    }
}

void BaseMessageItem::setUserName(const QString &name)
{
    if (!_nameLabel) {
        _nameLabel = new QLabel(this);
    }
    _nameLabel->setText(name);
    _nameLabel->setStyleSheet("font-weight: bold; color: #333;"); // 设置用户名样式
}
