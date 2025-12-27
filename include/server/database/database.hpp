#ifndef DATABASE_H
#define DATABASE_H

#include <mysql/mysql.h>
#include <iostream>
#include <memory>

using MySQLResPtr = std::unique_ptr<MYSQL_RES, decltype(&mysql_free_result)>;

static const char kDefaultHost[] = "127.0.0.1";
static const int kDefaultPort = 3306;
static const char kDefaultUser[] = "root";
static const char kDefaultPassword[] = "123456";
static const char kDefaultDbname[] = "chat";

/**
 * @brief MySQL数据库连接器类
 *
 * 封装了MySQL C API，提供面向对象的数据库操作接口
 */
class MySQLConn {
 public:
  MySQLConn();
  ~MySQLConn();
  // 数据库连接
  bool Connect(const std::string& ip = kDefaultHost,
               unsigned short port = kDefaultPort,
               const std::string& user = kDefaultUser,
               const std::string& password = kDefaultPassword,
               const std::string& dbname = kDefaultDbname);

  /**
   * @brief 执行SQL更新语句
   * @param sql 要执行的SQL语句（INSERT/UPDATE/DELETE）
   * @return true 执行成功
   * @return false 执行失败
   */
  bool Update(const std::string& sql);

  /**
   * @brief 执行SQL查询语句
   * @param sql 要执行的查询SQL语句
   * @return MysqlResPtr 查询结果集的智能指针
   */
  MySQLResPtr Query(const std::string& sql);

  /**
   * @brief 获取原始的MySQL连接对象
   * @return MYSQL* 指向MySQL连接结构体的指针
   * @note 谨慎使用，直接操作原始连接可能破坏封装性
   */
  MYSQL* GetConnection();

 private:
  MYSQL* conn_;
};

#endif