#ifndef FRIEND_DAO_H
#define FRIEND_DAO_H

#include <vector>
#include "user_entity.hpp"

class FriendDAO {
 public:
  void AddFriend(int user_id, int friend_id);
  std::vector<UserEntity> GetFriends(int user_id);
};

#endif