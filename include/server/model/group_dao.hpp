#ifndef GROUP_DAO_H
#define GROUP_DAO_H
#include <vector>
#include "group_entity.hpp"

class GroupDAO {
 public:
  // 创建群组
  bool CreateGroup(GroupEntity& group);
  // 加入群组
  bool JoinGroup(int user_id, int group_id, std::string role);
  // 查询用户所在群组信息
  std::vector<GroupEntity> GetUserGroups(int user_id);
  // 根据指定的groupid查询除userid以外的用户id列表
  std::vector<int> GetOtherGroupMembers(int group_id, int exclude_user_id);
};

#endif