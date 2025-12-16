#include "user_model.hpp"
#include "database.hpp"

bool UserModel::Insert(UserEntity& user) {
  // 1.组装sql语句
  char sql[1024];
  sprintf(sql,
          "insert into users(name, password, state) values('%s', '%s', '%s')",
          user.GetName().c_str(), user.GetPassword().c_str(),
          user.GetStatus().c_str());
  MySqlConn mysql_conn;
  if (mysql_conn.Connect()) {
    if (mysql_conn.Update(sql)) {
      // 获取插入成功的用户数据生成的ID
      user.SetID(mysql_insert_id(mysql_conn.GetConnection()));
      return true;
    }
  }
  return false;
}