#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <QObject>
#include <memory>
#include "singleton.h"
#include <vector>
#include "contactinfodata.h"

class UserManager : public QObject, public Singleton<UserManager>, public std::enable_shared_from_this<UserManager>
{
    Q_OBJECT
    friend class Singleton<UserManager>;
public:
    ~UserManager() = default;
    void setName(QString name);
    void setUid(int uid);
    void setToken(QString token);
    int getUid() const;
    QString getName() const;
    std::vector<std::shared_ptr<RequestInfo>> getFriendRequestList() const;
private:
    UserManager();
    QString _name;
    QString _token;
    int _uid;
    std::vector<std::shared_ptr<RequestInfo>> _friendRequestList;
};

#endif // USERMANAGER_H
