#ifndef GROUP_USER_ENTITY_H
#define GROUP_USER_ENTITY_H
#include "user_entity.hpp"

class GroupUserEntity : public UserEntity {
 public:
  void SetRole(std::string role) { role_ = role; }
  std::string GetRole() { return role_; }

 private:
  std::string role_;
};

#endif