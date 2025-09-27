#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QDialog>
#include <QPainter>
#include <QStyleOption>
#include "contactinfodata.h"
#include <QMap>

namespace Ui {
class ChatPage;
}

class ChatPage : public QDialog
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);
    void setPeerInfo(std::shared_ptr<SessionInfo> peerInfo);
    void AppendChatMessage(std::shared_ptr<TextChatData> message);
    ~ChatPage();
private slots:
    void on_send_btn_clicked();

private:
    virtual void paintEvent(QPaintEvent *event) override;
private:
    Ui::ChatPage *ui;
    std::shared_ptr<SessionInfo> _peerInfo; // 对方用户信息
signals:
    void signal_append_chat_message(std::shared_ptr<TextChatData> message); // 用于发送消息到聊天区域
private slots:

};

#endif // CHATPAGE_H
