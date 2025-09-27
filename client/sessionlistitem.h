#ifndef SESSIONLISTITEM_H
#define SESSIONLISTITEM_H
#include <QWidget>
#include "baselistitem.h"
#include "contactinfodata.h"

namespace Ui {
class SessionListItem;
}

class SessionListItem : public BaseListItem
{
    Q_OBJECT

public:
    explicit SessionListItem(QWidget *parent = nullptr);
    ~SessionListItem();
    QSize sizeHint() const override;
    void setSessionUI(const QString &sessionName, const QString & iconPath, const QString &message);
    void setFullSessionInfo(std::shared_ptr<SessionInfo> sessionInfo);
    void updateSessionItemLastMsg(std::vector<std::shared_ptr<TextChatData>> messageBatch); // 更新会话项的最后一条消息内容
    std::shared_ptr<SessionInfo> getSessionInfo() const { return _sessionInfo; } // 获取会话信息
private:
    Ui::SessionListItem *ui;
    QString _sessionName;
    QString _iconPath;
    QString _messageContent;
    std::shared_ptr<SessionInfo> _sessionInfo; // 会话信息
};

#endif // SESSIONLISTITEM_H
