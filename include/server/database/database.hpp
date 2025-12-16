#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>
#include <iostream>
#include <memory>

using MysqlResPtr = std::unique_ptr<MYSQL_RES, decltype(&mysql_free_result)>;

static const char kDefaultHost[] = "127.0.0.1";
static const int kDefaultPort = 3306;
static const char kDefaultUser[] = "root";
static const char kDefaultPassword[] = "123456";
static const char kDefaultDbname[] = "chat";

class MySqlConn {
 public:
  MySqlConn();
  ~MySqlConn();
  // 数据库连接
  bool Connect(const std::string& ip = kDefaultHost,
               unsigned short port = kDefaultPort,
               const std::string& user = kDefaultUser,
               const std::string& password = kDefaultPassword,
               const std::string& dbname = kDefaultDbname);

  // 更新操作
  bool Update(const std::string& sql);
  // 查询操作
  MysqlResPtr Query(const std::string& sql);
  // 获取连接
  MYSQL* GetConnection();

 private:
  MYSQL* conn_;
};

#endif