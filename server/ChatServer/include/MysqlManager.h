#pragma once
#include "MysqlDAO.h"
#include "Singleton.h"
#include <spdlog/spdlog.h>

/*
    用于在LogicSystem进行调用，内含用户注册、登录等功能
    操作MysqlDAO
*/
class MysqlManager : public Singleton<MysqlManager> {
    friend class Singleton<MysqlManager>;
public:
    ~MysqlManager() = default;
    int RegisterUser(const std::string& name, const std::string& email,  const std::string& password);
    bool CheckEmailAndUserMatch(const std::string& name, const std::string& email);
    bool CheckEmailAndPasswordMatch(const std::string& email, const std::string& password, UserInfo& user_info);
    bool ResetPassword(const std::string& name, const std::string& new_password);
    UserInfo GetUserInfo(int uid);
    std::shared_ptr<FullUserInfo> getFullUserInfoByUid(int uid);
    std::shared_ptr<FullUserInfo> getFullUserInfoByName(const std::string& name);
    bool CheckEmailExists(const std::string& email);
    bool AddItemToFriendRequestList(int from_uid, int to_uid);
    bool RemoveItemFromFriendRequestList(int from_uid, int to_uid);
    bool AcceptAndUpdateFriendRequestListItemStatus(int from_uid, int to_uid, int status); // meaning the request is accepted and the status is updated
    bool AddBidirectionalFriendRelationship(int uid1, int uid2, const std::string& backup_name_1_to_2, const std::string& backup_name_2_to_1);
    bool AddOneWayFriendRelationship(int uid1, int uid2, const std::string& backup_name_1_to_2);
    bool RemoveOneWayFriendRelationship(int uid1, int uid2); // Remove one way friend relationship,用于错误发生时的回滚
    bool IsFriendRequestExistsByCheckOneWay(int from_uid, int to_uid);
    bool IsFriendAlreadyByCheckOneWay(int uid1, int uid2); // 这个函数检查单向的好友请求是否存在
    bool IsFriendAlreadyByCheckTwoWay(int uid1, int uid2); // 这个函数检查两边的好友关系是否都存在
    int GetFriendRequestList(int self_uid, std::vector<FriendRequestItem>& friend_request_list);
    int GetFriendList(int self_uid, std::vector<FriendInfo>& friend_list);
private:
    MysqlManager();
    MysqlManager(const MysqlManager&) = delete;
    MysqlManager& operator=(const MysqlManager&) = delete;
private:
    std::unique_ptr<MysqlDAO> _dao;
};