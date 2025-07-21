#include "contactinfodata.h"

SearchInfo::SearchInfo(int uid, QString name, QString nickname, QString description, int gender) :
    _uid(uid),
    _name(name),
    _nickname(nickname),
    _description(description),
    _gender(gender)
{

}

int SearchInfo::getUid() const
{
    return _uid;
}

QString SearchInfo::getName() const
{
    return _name;
}

QString SearchInfo::getNickname() const
{
    return _nickname;
}

QString SearchInfo::getDescription() const
{
    return _description;
}

int SearchInfo::getGender() const
{
    return _gender;
}
