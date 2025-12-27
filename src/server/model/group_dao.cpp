#include "group_dao.hpp"
#include "database.hpp"

// 创建群组
bool GroupDAO::CreateGroup(GroupEntity& group) {
  char sql[1024] = {0};
  sprintf(sql, "insert into chat_group(groupname, groupdesc) values('%s','%s')",
          group.GetName().c_str(), group.GetDesc().c_str());
  MySQLConn mysql_conn;
  if (mysql_conn.Connect()) {
    if (mysql_conn.Update(sql)) {
      group.SetId(mysql_insert_id(mysql_conn.GetConnection()));
      return true;
    }
  }
  return false;
}

// 加入群组
bool GroupDAO::JoinGroup(int user_id, int group_id, std::string role) {
  char sql[1024] = {0};
  sprintf(sql,
          "insert into group_member(userid, groupid, grouprole) values(%d, %d, "
          "'%s')",
          user_id, group_id, role.c_str());
  MySQLConn mysql_conn;
  if (mysql_conn.Connect()) {
    if (mysql_conn.Update(sql)) {
      return true;
    }
  }
  return false;
}

// 查询用户所在群组信息
std::vector<GroupEntity> GroupDAO::GetUserGroups(int user_id) {
  char sql[1024] = {0};
  sprintf(sql,
          "select a.id, a.name, a.desc from chat_group a inner join "
          "group_member b on a.id = b.groupid where b.userid = %d",
          user_id);
  std::vector<GroupEntity> group_vec;
  MySQLConn mysql_conn;
  if (mysql_conn.Connect()) {
    MySQLResPtr res_ptr = mysql_conn.Query(sql);
    if (res_ptr != nullptr) {
      MYSQL_ROW row;
      while ((row = mysql_fetch_row(res_ptr.get())) != nullptr) {
        if (row != nullptr) {
          GroupEntity group;
          group.SetId(atoi(row[0]));
          group.SetName(row[1]);
          group.SetDesc(row[2]);
          group_vec.push_back(group);
        }
      }
    }
  }
  // 查询群组的用户信息
  for (auto& group : group_vec) {
    sprintf(sql,
            "selcet a.id, a.name, a.state, b.grouprole from user a inner join "
            "group_member b on a.id = b.userid where b.groupid = %d",
            group.GetId());
    MySQLResPtr res_ptr = mysql_conn.Query(sql);
    if (res_ptr != nullptr) {
      MYSQL_ROW row;
      while ((row = mysql_fetch_row(res_ptr.get())) != nullptr) {
        if (row != nullptr) {
          GroupUserEntity user;
          user.SetId(atoi(row[0]));
          user.SetName(row[1]);
          user.SetState(row[2]);
          user.SetRole(row[3]);
          group.GetGroupUsers().push_back(user);
        }
      }
    }
  }
  return group_vec;
}

// 根据指定的groupid查询除userid以外的用户id列表
std::vector<int> GroupDAO::GetOtherGroupMembers(int group_id,
                                                int exclude_user_id) {
  char sql[1024] = {0};
  sprintf(sql,
          "selcet userid from group_member where groupid = %d and userid != %d",
          group_id, exclude_user_id);
  std::vector<int> group_members;
  MySQLConn mysql_conn;
  if (mysql_conn.Connect()) {
    MySQLResPtr res_ptr = mysql_conn.Query(sql);
    if (res_ptr != nullptr) {
      MYSQL_ROW row;
      while ((row = mysql_fetch_row(res_ptr.get())) != nullptr) {
        if (row != nullptr) {
          group_members.push_back(atoi(row[0]));
        }
      }
    }
  }
  return group_members;
}