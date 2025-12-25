#ifndef USER_DAO_H
#define USER_DAO_H

#include "user_entity.hpp"

class UserDAO {
 public:
  bool Insert(UserEntity& user);
  UserEntity Query(int id);
  bool UpdateState(UserEntity& user);
  void ResetState();

 private:
};

#endif