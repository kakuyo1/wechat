#ifndef CONTACTINFODATA_H
#define CONTACTINFODATA_H
#include <QString>

class SearchInfo {
public:
    SearchInfo(int uid, QString name, QString nickname, QString description, int gender);
    ~SearchInfo() = default;
    int getUid() const;
    QString getName() const;
    QString getNickname() const;
    QString getDescription() const;
    int getGender() const;
private:
    int _uid; // 用户ID
    QString _name; // 用户名
    QString _nickname; // 昵称
    QString _description; // 描述
    int _gender; // 性别
};

#endif // CONTACTINFODATA_H
