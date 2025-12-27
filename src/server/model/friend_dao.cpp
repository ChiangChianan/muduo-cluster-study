#include "friend_dao.hpp"
#include <iostream>
#include "database.hpp"

void FriendDAO::AddFriend(int user_id, int friend_id) {
  char sql[1024];
  sprintf(sql, "insert into friend values(%d, %d)", user_id, friend_id);
  MySQLConn mysql_conn;
  if (mysql_conn.Connect()) {
    mysql_conn.Update(sql);
  }
}

std::vector<UserEntity> FriendDAO::GetFriends(int user_id) {
  char sql[1024];
  sprintf(sql,
          "select a.id, a.name, a.state from user a inner join friend b on "
          "b.friendid = a.id where b.userid = %d",
          user_id);
  MySQLConn mysql_conn;
  std::vector<UserEntity> res;
  if (mysql_conn.Connect()) {
    MySQLResPtr res_ptr = mysql_conn.Query(sql);
    if (res_ptr != nullptr) {
      MYSQL_ROW row;
      while ((row = mysql_fetch_row(res_ptr.get())) != nullptr) {
        UserEntity user;
        user.SetId(atoi(row[0]));
        user.SetName(row[1]);
        user.SetState(row[2]);
        res.push_back(user);
      }
    }
  }
  return res;
}