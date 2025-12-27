#ifndef USER_ENTITY_H
#define USER_ENTITY_H
#include <iostream>

/**
 * @brief 用户实体类，映射数据库 user 表的字段
 * 负责在内存中暂存用户信息，不包含业务逻辑和数据库操作代码。
 * 对应数据库字段：id, name, password, state (status)。
 */

class UserEntity {
 public:
  UserEntity(int id = -1, std::string name = "", std::string password = "",
             std::string state = "offline")
      : id_(id),
        name_(std::move(name)),
        password_(std::move(password)),
        state_(std::move(state)) {}
  void SetId(int id) { id_ = id; }
  void SetName(std::string name) { name_ = name; }
  void SetPassword(std::string password) { password_ = password; }
  void SetState(std::string state) { state_ = state; }

  int GetId() { return id_; }
  std::string GetName() { return name_; }
  std::string GetPassword() { return password_; }
  std::string GetState() { return state_; }

 private:
  int id_;
  std::string name_;
  std::string password_;
  std::string state_;
};

#endif