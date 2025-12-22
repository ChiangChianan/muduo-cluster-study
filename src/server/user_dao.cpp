#include "user_dao.hpp"
#include "database.hpp"

bool UserDAO::Insert(UserEntity& user) {
  // 1.组装sql语句
  char sql[1024];
  sprintf(sql,
          "insert into users(name, password, state) values('%s', '%s', '%s')",
          user.GetName().c_str(), user.GetPassword().c_str(),
          user.GetState().c_str());
  MySQLConn mysql_conn;
  if (mysql_conn.Connect()) {
    if (mysql_conn.Update(sql)) {
      // 获取插入成功的用户数据生成的ID
      user.SetID(mysql_insert_id(mysql_conn.GetConnection()));
      return true;
    }
  }
  return false;
}

UserEntity UserDAO::Query(int id) {
  char sql[1024];
  sprintf(sql, "select * from users where id = %d", id);
  MySQLConn mysql_conn;
  if (mysql_conn.Connect()) {
    MySQLResPtr res_ptr = mysql_conn.Query(sql);
    if (res_ptr != nullptr) {
      MYSQL_ROW row = mysql_fetch_row(res_ptr.get());
      if (row != nullptr) {
        UserEntity user;
        user.SetID(atoi(row[0]));
        user.SetName(row[1]);
        user.SetPassword(row[2]);
        user.SetState(row[3]);
        return user;
      }
    }
  }
  return UserEntity();
}

bool UserDAO::UpdateState(UserEntity& user) {
  char sql[1024];
  sprintf(sql, "update users set state = '%s' where id = %d",
          user.GetState().c_str(), user.GetID());
  MySQLConn mysql_conn;
  if (mysql_conn.Connect()) {
    if (mysql_conn.Update(sql)) {
      // 获取插入成功的用户数据生成的ID
      return true;
    }
  }
  return false;
}

void UserDAO::ResetState() {
  char sql[1024] = "update users set state = 'offline' where state = 'online'";
  MySQLConn mysql_conn;
  if (mysql_conn.Connect()) {
    mysql_conn.Update(sql);
  }
}