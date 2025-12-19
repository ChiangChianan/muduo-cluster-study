#ifndef USER_MODEL_H
#define USER_MODEL_H

#include "user_entity.hpp"

class UserModel {
 public:
  bool Insert(UserEntity& user);
  UserEntity Query(int id);
  bool UpdateState(UserEntity& user);

 private:
};

#endif