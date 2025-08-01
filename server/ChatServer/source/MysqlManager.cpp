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

bool MysqlManager::RemoveItemFromFriendRequestList(int from_uid, int to_uid) {
    return _dao->RemoveItemFromFriendRequestList(from_uid, to_uid);
}

bool MysqlManager::AcceptAndUpdateFriendRequestListItemStatus(int from_uid, int to_uid, int status) {
    return _dao->AcceptAndUpdateFriendRequestListItemStatus(from_uid, to_uid, status);
}

bool MysqlManager::AddBidirectionalFriendRelationship(int uid1, int uid2, const std::string& backup_name_1_to_2, const std::string& backup_name_2_to_1) {
    return _dao->AddBidirectionalFriendRelationship(uid1, uid2, backup_name_1_to_2, backup_name_2_to_1);
}

int MysqlManager::GetFriendRequestList(int self_uid, std::vector<FriendRequestItem> &friend_request_list)
{
    return _dao->GetFriendRequestList(self_uid, friend_request_list);
}

int MysqlManager::GetFriendList(int self_uid, std::vector<FriendInfo> &friend_list)
{
    return _dao->GetFriendList(self_uid, friend_list);
}

bool MysqlManager::AddOneWayFriendRelationship(int uid1, int uid2, const std::string& backup_name_1_to_2) {
    return _dao->AddOneWayFriendRelationship(uid1, uid2, backup_name_1_to_2);
}

bool MysqlManager::RemoveOneWayFriendRelationship(int uid1, int uid2) {
    return _dao->RemoveOneWayFriendRelationship(uid1, uid2);
}

bool MysqlManager::IsFriendRequestExistsByCheckOneWay(int from_uid, int to_uid) {
    return _dao->IsFriendRequestExistsByCheckOneWay(from_uid, to_uid);
}

bool MysqlManager::IsFriendAlreadyByCheckOneWay(int uid1, int uid2) {
    return _dao->IsFriendAlreadyByCheckOneWay(uid1, uid2);
}

bool MysqlManager::IsFriendAlreadyByCheckTwoWay(int uid1, int uid2) {
    return _dao->IsFriendAlreadyByCheckTwoWay(uid1, uid2);
}