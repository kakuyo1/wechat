#include "../include/MysqlManager.h"

MysqlManager::MysqlManager() :
    _dao(std::make_unique<MysqlDAO>())
{

}

int MysqlManager::RegisterUser(const std::string& name, const std::string& email,  const std::string& password) {
    return _dao->RegisterUser(name, email, password);
}

bool MysqlManager::CheckEmailAndUserMatch(const std::string& name, const std::string& email) {
    return _dao->CheckEmailAndUserMatch(name, email);
}

bool MysqlManager::CheckEmailAndPasswordMatch(const std::string &email, const std::string &password, UserInfo& user_info)
{
    return _dao->CheckEmailAndPasswordMatch(email, password, user_info);
}

bool MysqlManager::ResetPassword(const std::string& name, const std::string& new_password) {
    return _dao->ResetPassword(name, new_password);
}

UserInfo MysqlManager::GetUserInfo(int uid) {
    return _dao->GetUserInfo(uid);
}

bool MysqlManager::CheckEmailExists(const std::string &email)
{
    return _dao->CheckEmailExists(email);
}

std::shared_ptr<FullUserInfo> MysqlManager::getFullUserInfoByUid(int uid)
{
    return _dao->getFullUserInfoByUid(uid);
}

std::shared_ptr<FullUserInfo> MysqlManager::getFullUserInfoByName(const std::string& name)
{
    return _dao->getFullUserInfoByName(name);
}

bool MysqlManager::AddItemToFriendRequestList(int from_uid, int to_uid) {
    return _dao->AddItemToFriendRequestList(from_uid, to_uid);
}
bool MysqlManager::AccpetAndUpdateFriendRequestListItemStatus(int from_uid, int to_uid) {
    return _dao->AccpetAndUpdateFriendRequestListItemStatus(from_uid, to_uid);
}
bool MysqlManager::AddBidirectionalFriendRelationship(int uid1, int uid2, const std::string& backup_name_1_to_2, const std::string& backup_name_2_to_1) {
    return _dao->AddBidirectionalFriendRelationship(uid1, uid2, backup_name_1_to_2, backup_name_2_to_1);
}