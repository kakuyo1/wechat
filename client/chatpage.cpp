#include "chatpage.h"
#include "ui_chatpage.h"
#include "textframe.h"
#include "customized_textedit.h"
#include "basemessageitem.h"

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
    // Test
    QString text = ui->chat_textEdit->toPlainText();
    if (text.isEmpty()) {
        return; // 如果文本为空，直接返回
    }
    // 测试参数
    MessageRole role = MessageRole::SELF;
    QString userName = "Test User";
    QString avatarPath = ":/images/head_1.jpg";
    // 从文本编辑器获取所有消息向量Vector
    const QVector<messageInfo> &messages = ui->chat_textEdit->createAndGetAllMessages();
    for (const auto& message : messages) {
        BaseMessageItem* item = new BaseMessageItem(role);
        QWidget* BubbleWidget = nullptr;
        item->setUserName(userName);
        item->setUserAvatar(avatarPath);
        if (message.messageType == "text") {
            // 创建文本气泡
            BubbleWidget = new TextFrame(role, message.messageContent); // 多态
        } else if (message.messageType == "image") {
            qDebug() << "Image message type not implemented yet.";
        } else {
            qDebug() << "Unknown message type:" << message.messageType;
        }
        // 添加到聊天区域
        if (BubbleWidget != nullptr) {
            item->setBubbleWidget(BubbleWidget);
            ui->chat_view->appendMessageWidget(item);
        }
    }
}
