#include "contactinfodata.h"

SearchInfo::SearchInfo(int uid, QString name, QString nickname, QString description, int gender, QString avatarPath, QString emial) :
    _uid(uid),
    _name(name),
    _nickname(nickname),
    _description(description),
    _gender(gender),
    _avatarPath(avatarPath),
    _emial(emial)
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


AddContactRequest::AddContactRequest(int fromUid, QString fromName, QString fromNickname,
                                     QString fromAvatarPath, QString fromDescription, int fromGender, QString fromEmail) :
    _fromUid(fromUid), _fromGender(fromGender), _fromName(fromName), _fromNickname(fromNickname), _fromAvatarPath(fromAvatarPath)
    , _fromDescription(fromDescription),  _fromEmail(fromEmail)
{

}

RequestInfo::RequestInfo(int uid, QString name, QString nickname, QString avatarPath, QString description, int gender, int status) :
    _uid(uid), _gender(gender), _name(name), _nickname(nickname), _description(description), _avatarPath(avatarPath)
     , _status(status)
{

}

RequestInfo::RequestInfo(std::shared_ptr<AddContactRequest> addContactRequest) :
    _uid(addContactRequest->_fromUid),
    _gender(addContactRequest->_fromGender),
    _name(addContactRequest->_fromName),
    _nickname(addContactRequest->_fromNickname),
    _description(addContactRequest->_fromDescription),
    _avatarPath(addContactRequest->_fromAvatarPath),
    _status(0) // 默认状态为0，表示未处理)
{

}

AuthRequest::AuthRequest(int uid, QString name, QString nickname, QString avatarPath, int gender) :
    _uid(uid), _name(name), _nickname(nickname), _icon(avatarPath), _gender(gender)
{

}

AuthResponse::AuthResponse(int peeruid, int peergender, QString peername, QString peernickname,
                           QString peericon, QString peeremail, QString peerdescription) :
    _peeruid(peeruid), _peergender(peergender),  _peername(peername), _peernickname(peernickname), _peericon(peericon), _peeremail(peeremail), _peerdescription(peerdescription)
{

}

AddContactResponse::AddContactResponse(int Uid, QString Name, QString Nickname, QString AvatarPath, QString Description, int Gender, QString Email) :
    _uid(Uid), _gender(Gender), _name(Name), _nickname(Nickname), _description(Description),  _avatarPath(AvatarPath),
    _email(Email)
{

}

FriendListItemInfo::FriendListItemInfo(int self_uid, int request_uid, QString name,
                                       QString desc, QString avatarPath, int status) :
    _selfUid(self_uid), _requestUid(request_uid), _name(name), _desc(desc), _avatarPath(avatarPath), _status(status)
{

}
