#include "chatpage.h"
#include "ui_chatpage.h"
#include "textframe.h"
#include "customized_textedit.h"
#include "basemessageitem.h"
#include "pictureframe.h"
#include <QUuid>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "usermanager.h"
#include <QMessageBox>

ChatPage::ChatPage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatPage)
{
    ui->setupUi(this);
    ui->emoji_label->setState("normal", "hover", "pressed");
    ui->file_label->setState("normal", "hover", "pressed");
    ui->send_btn->setState("normal", "hover", "pressed");
    ui->receive_btn->setState("normal", "hover", "pressed");
}
// 在点击会话项后设置对方信息
void ChatPage::setPeerInfo(std::shared_ptr<SessionInfo> peerInfo)
{
    if (!peerInfo) {
        qDebug() << "peerInfo is null";
        return;
    }
    _peerInfo = peerInfo;
    // 设置UI
    ui->title_label->setText(_peerInfo->_peername);
    qDebug() << "A";
    ui->chat_view->removeAllitem(); // 清空聊天区域
    qDebug() << "B";
    // 添加/刷新历史消息
    for (const auto& message : _peerInfo->_chatHistory) {
        AppendChatMessage(message);
        qDebug() << "C";
    }
}
// 添加一条聊天消息到聊天区域
void ChatPage::AppendChatMessage(std::shared_ptr<TextChatData> message)
{
    auto self_uid = UserManager::GetInstance()->getUid();
    auto self_name = UserManager::GetInstance()->getName();
    auto self_avatar = UserManager::GetInstance()->getIconPath();
    MessageRole role;
    if (message->_from_uid == self_uid) {
        role = MessageRole::SELF; // 自己发送的消息
        // 创建消息项
        BaseMessageItem* item = new BaseMessageItem(role);
        item->setUserName(self_name);
        item->setUserAvatar(self_avatar);
        // 创建文本气泡
        QWidget* BubbleWidget = new TextFrame(role, message->_message_content);
        item->setBubbleWidget(BubbleWidget);
        // 添加消息控件到布局中
        ui->chat_view->appendMessageWidget(item);
    } else {
        role = MessageRole::OTHER; // 对方发送的消息
        // 创建消息项
        BaseMessageItem* item = new BaseMessageItem(role);
        item->setUserName(_peerInfo->_peername); // 在点击会话项SessionItem时就设置了对方信息peerInfo
        item->setUserAvatar(_peerInfo->_peericon);
        // 创建文本气泡
        QWidget* BubbleWidget = new TextFrame(role, message->_message_content);
        item->setBubbleWidget(BubbleWidget);
        // 添加到聊天区域
        ui->chat_view->appendMessageWidget(item);
    }
}

ChatPage::~ChatPage()
{
    delete ui;
}

void ChatPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void ChatPage::on_send_btn_clicked()
{
    if (ui->chat_textEdit->toPlainText().isEmpty()) {
        return; // 如果文本编辑器为空，直接返回
    }

    if (!_peerInfo) {
        qDebug() << "Peer info is not set!";
        return; // 如果对方信息未设置，直接返回
    }

    // 拿到自己的信息
    QString self_name = UserManager::GetInstance()->getName();
    QString self_avatar = UserManager::GetInstance()->getIconPath();
    int self_uid = UserManager::GetInstance()->getUid();
    MessageRole role = MessageRole::SELF; // 自己发送的消息
    auto TextEdit = ui->chat_textEdit;

    // 获取文本编辑器中的消息内容
    const QVector<messageInfo>& messageList = TextEdit->createAndGetAllMessages();
    QJsonObject jsonObject;
    QJsonArray messagesArray;
    int totalMessagesSentSize = 0;
    // 遍历所有消息
    for (const auto& message : messageList) {
        // 单条消息长度超过1024则视为违规，跳过
        if (message.messageContent.size() > 1024) {
            QMessageBox::warning(this, "Warning", "Message exceeds the maximum length of 1024 characters.");
            continue;
        }
        // 创建消息项
        QString message_type = message.messageType;
        QString message_content = message.messageContent;
        BaseMessageItem* item = new BaseMessageItem(role);
        item->setUserName(self_name);
        item->setUserAvatar(self_avatar);
        QWidget* BubbleWidget = nullptr;
        // 根据不同消息类型，采取不同策略
        if (message_type == "text") {
            // 为消息生成UUID
            QUuid uuid = QUuid::createUuid();
            QString message_uuid = uuid.toString();
            // 创建文本气泡
            BubbleWidget = new TextFrame(role, message_content);
            // 如果当前累计消息长度超过 1024，则把已经累积的消息打包发送。
            // 多条消息合起来太长，分段发送，防止一次发送太大(多条消息合起来不足1024字节将跳过，直接进入最后的收尾发送)
            if (totalMessagesSentSize + message_content.size() > 1024) {
                jsonObject["from_uid"] = self_uid;
                jsonObject["to_uid"] = _peerInfo->_peeruid;
                jsonObject["text_array"] = messagesArray; // 添加消息数组到JSON对象

                QJsonDocument doc(jsonObject);
                QByteArray byteArray = doc.toJson(QJsonDocument::Compact);
               // 清空缓存，准备下一批
                totalMessagesSentSize = 0;
                messagesArray = QJsonArray();
                jsonObject = QJsonObject(); // 清空JSON对象
                // 用TCP发送消息
                emit TcpManager::GetInstance()->signal_send_data(RequestType::MESSAGE_CLIENT_CHATTEXT_REQUEST, byteArray); // 给别人看
            }
            // 这里继续累积消息（前面没有任何消息发送时，应该从这里开始）
            totalMessagesSentSize += message_content.size();
            QJsonObject messageObject;
            QByteArray utf8Content = message_content.toUtf8();
            QString utf8ContentStr = QString::fromUtf8(utf8Content);
            messageObject["message_uuid"] = message_uuid;
            messageObject["message_content"] = utf8ContentStr; // 使用UTF-8编码
            messagesArray.append(messageObject);
            std::shared_ptr<TextChatData> chatMessage = std::make_shared<TextChatData>(message_uuid, utf8ContentStr, self_uid, _peerInfo->_peeruid);

            emit signal_append_chat_message(chatMessage); // 更新自己和对方的聊天记录
        } else if (message_type == "image") {
            // 创建图片气泡
            BubbleWidget = new PictureFrame(message.image, role);
            // TODO
        } else if (message_type == "file") {
            // TODO
        } else {
            qDebug() << "Unknown message type:" << message_type;
            continue; // 跳过未知消息类型
        }
        // 在VIEW添加消息项
        if (BubbleWidget != nullptr) {
            item->setBubbleWidget(BubbleWidget);
            ui->chat_view->appendMessageWidget(item); // 添加到聊天区域, 给自己看
        }
    }
    // 消息循环结束后,收尾发送,把剩下未满 1024 的也发出去
    if (!messagesArray.isEmpty()) {
        jsonObject["from_uid"] = self_uid;
        jsonObject["to_uid"] = _peerInfo->_peeruid;
        jsonObject["text_array"] = messagesArray; // 添加消息数组到JSON对象

        QJsonDocument doc(jsonObject);
        QByteArray byteArray = doc.toJson(QJsonDocument::Compact);
        // 清空累积的文本列表
        totalMessagesSentSize = 0;
        messagesArray = QJsonArray();
        jsonObject = QJsonObject(); // 清空JSON对象
        // 用TCP发送消息
        emit TcpManager::GetInstance()->signal_send_data(RequestType::MESSAGE_CLIENT_CHATTEXT_REQUEST, byteArray); // 给别人看
    }
}
