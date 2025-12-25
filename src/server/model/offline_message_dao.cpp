#include "offline_message_dao.hpp"
#include "database.hpp"

// 存储离线消息
void OfflineMessageDAO::StoreOfflineMessage(int user_id,
                                            const std::string& message) {
  char sql[1024];
  sprintf(sql, "insert into offline_message values(%d, '%s')", user_id,
          message.c_str());
  MySQLConn mysql_conn;
  if (mysql_conn.Connect()) {
    mysql_conn.Update(sql);
  }
}

// 删除指定用户的离线消息
void OfflineMessageDAO::DeleteOfflineMessageByUserId(int user_id) {
  char sql[1024];
  sprintf(sql, "delete from offline_message where userid =  %d", user_id);
  MySQLConn mysql_conn;
  if (mysql_conn.Connect()) {
    mysql_conn.Update(sql);
  }
}

// 查询离线消息
std::vector<std::string> OfflineMessageDAO::QueryOfflineMessages(int user_id) {
  char sql[1024];
  sprintf(sql, "select message from offline_message where userid = %d",
          user_id);
  MySQLConn mysql_conn;
  std::vector<std::string> res;
  if (mysql_conn.Connect()) {
    MySQLResPtr res_ptr = mysql_conn.Query(sql);
    if (res_ptr != nullptr) {
      MYSQL_ROW row;
      while ((row = mysql_fetch_row(res_ptr.get())) != nullptr)
        res.push_back(row[0]);
    }
  }
  return res;
}