#include "database.hpp"
#include <muduo/base/Logging.h>

MySQLConn::MySQLConn() {
  conn_ = mysql_init(nullptr);
  if (conn_ == nullptr) {
    LOG_ERROR << "MySQL init failed!";
  }
}

MySQLConn::~MySQLConn() {
  if (conn_ != nullptr) {
    mysql_close(conn_);
    conn_ = nullptr;
  }
}

bool MySQLConn::Connect(const std::string& ip, unsigned short port,
                        const std::string& user, const std::string& password,
                        const std::string& dbname) {
  MYSQL* p =
      mysql_real_connect(conn_, ip.c_str(), user.c_str(), password.c_str(),
                         dbname.c_str(), port, nullptr, 0);
  if (p != nullptr) {
    mysql_set_character_set(conn_, "utf8");
    LOG_INFO << "Connection sucessful ";
    return true;
  }
  LOG_ERROR << "Connection failed: " << mysql_error(conn_);
  return false;
}

bool MySQLConn::Update(const std::string& sql) {
  if (mysql_query(conn_, sql.c_str())) {
    LOG_ERROR << "Update error: " << sql << " -> " << mysql_error(conn_);
    return false;
  } else {
    return true;
  }
}

MySQLResPtr MySQLConn::Query(const std::string& sql) {
  if (mysql_query(conn_, sql.c_str())) {
    LOG_ERROR << "Query error: " << sql << " -> " << mysql_error(conn_);
    return MySQLResPtr(nullptr, mysql_free_result);
  }
  return MySQLResPtr(mysql_store_result(conn_), mysql_free_result);
}

MYSQL* MySQLConn::GetConnection() { return conn_; }