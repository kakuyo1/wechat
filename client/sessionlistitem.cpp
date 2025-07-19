#include "sessionlistitem.h"
#include "ui_sessionlistitem.h"

SessionListItem::SessionListItem(QWidget *parent)
    : BaseListItem(parent)
    ,ui(new Ui::SessionListItem)
    , _sessionName("")
    , _iconPath("")
    , _messageContent("")
{
    ui->setupUi(this);
    // 设置会话类型
    setListItem(ListItemType::ChatItem);
}

SessionListItem::~SessionListItem()
{
    delete ui;
}

QSize SessionListItem::sizeHint() const
{
    return QSize(250, 70);
}

void SessionListItem::setSessionInfo(const QString &sessionName, const QString &iconPath, const QString &message)
{
    _sessionName = sessionName;
    _iconPath = iconPath;
    _messageContent = message;

    // (函数)字数超过20个字符，截断显示并加上...
    auto truncateText = [](const QString &text, int maxLength) {
        if (text.length() > maxLength) {
            return text.left(maxLength) + "...";
        }
        return text;
    };

    // 设置头像自动缩放
    ui->icon_label->setPixmap(QPixmap(_iconPath).scaled(ui->icon_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_label->setScaledContents(true);

    // 设置会话名称和消息内容，超过长度则截断
    ui->user_chatinfo_label->setText(truncateText(_messageContent, 20));
    ui->user_name_label->setText(truncateText(_sessionName, 10));
}
