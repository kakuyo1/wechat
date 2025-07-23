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

UserInfo::UserInfo(int uid, QString name, QString avatarPath) :
    _uid(uid),
    _name(name),
    _avatarPath(avatarPath)
{

}

AddContactRequest::AddContactRequest(int fromUid, QString fromName, QString fromNickname,
                                     QString fromAvatarPath, QString fromDescription, int fromGender) :
    _fromUid(fromUid), _fromName(fromName), _fromNickname(fromNickname), _fromAvatarPath(fromAvatarPath),
    _fromDescription(fromDescription), _fromGender(fromGender)
{

}

RequestInfo::RequestInfo(int uid, QString name, QString nickname, QString avatarPath, QString description, int gender, int status) :
    _uid(uid), _name(name), _nickname(nickname), _description(description), _avatarPath(avatarPath),
     _gender(gender), _status(status)
{

}

RequestInfo::RequestInfo(std::shared_ptr<AddContactRequest> addContactRequest) :
    _uid(addContactRequest->_fromUid),
    _name(addContactRequest->_fromName),
    _nickname(addContactRequest->_fromNickname),
    _description(addContactRequest->_fromDescription),
    _avatarPath(addContactRequest->_fromAvatarPath),
    _gender(addContactRequest->_fromGender),
    _status(0) // 默认状态为0，表示未处理)
{

}

AuthRequest::AuthRequest(int uid, QString name, QString nickname, QString avatarPath, int gender) :
    _uid(uid), _name(name), _nickname(nickname), _icon(avatarPath), _gender(gender)
{

}

AuthResponse::AuthResponse(int peeruid, QString peername, QString peernickname, QString peericon, int peergender) :
    _peeruid(peeruid), _peername(peername), _peernickname(peernickname), _peericon(peericon), _peergender(peergender)
{

}
