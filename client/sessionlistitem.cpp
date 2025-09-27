#include "sessionlistitem.h"
#include "ui_sessionlistitem.h"

#include <QDir>

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

void SessionListItem::setSessionUI(const QString &sessionName, const QString &iconPath, const QString &message)
{
    _sessionName = sessionName;
    // 从static文件夹中获取要添加的头像(Icon path: "B:\\qt learing\\llfc_client\\build\\Desktop_Qt_6_9_0_MinGW_64_bit-Debug\\debug\\static\\head_1.jpg")
    QString appPath = QCoreApplication::applicationDirPath();
    QString final_iconPath = QDir::toNativeSeparators(appPath + QDir::separator() + "static" + QDir::separator() + iconPath);
    _iconPath = final_iconPath;
    _messageContent = message;

    // (函数)字数超过20个字符，截断显示并加上...
    auto truncateText = [](const QString &text, int maxLength) {
        if (text.length() > maxLength) {
            return text.left(maxLength) + "...";
        }
        return text;
    };
    qDebug() << "Setting session UI: " << _sessionName << ", Icon: " << _iconPath << ", Message: " << _messageContent;
    // 设置头像自动缩放
    ui->icon_label->setPixmap(QPixmap(_iconPath).scaled(ui->icon_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_label->setScaledContents(true);
    qDebug() << "Icon path set to: " << _iconPath;
    // 设置会话名称和消息内容，超过长度则截断
    ui->user_chatinfo_label->setText(truncateText(_messageContent, 20));
    ui->user_name_label->setText(truncateText(_sessionName, 10));
}

void SessionListItem::setFullSessionInfo(std::shared_ptr<SessionInfo> sessionInfo)
{
    _sessionInfo = sessionInfo;
}

void SessionListItem::updateSessionItemLastMsg(std::vector<std::shared_ptr<TextChatData> > messageBatch)
{
    if (messageBatch.empty()) {
        return; // 如果消息批次为空，直接返回
    }
    // 获取最后一条消息
    QString lastMessageContent = "";
    for (const auto& message: messageBatch) {
        if (message) {
            lastMessageContent = message->_message_content; // 获取消息内容
            _sessionInfo->_chatHistory.push_back(message); // 添加到会话历史
        }
    }
    _sessionInfo->_last_message_to_show = lastMessageContent; // 更新会话项的最后一条消息内容
    ui->user_chatinfo_label->setText(lastMessageContent); // 更新UI显示的最后一条消息内容
}
