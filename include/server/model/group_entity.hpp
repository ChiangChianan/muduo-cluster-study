#ifndef GROUP_ENTITY_H
#define GROUP_ENTITY_H

#include <iostream>
#include <vector>
#include "group_user_entity.hpp"

class GroupEntity {
 public:
  GroupEntity(int id = -1, const std::string& name = "",
              const std::string& desc = "")
      : id_(id), name_(name), desc_(desc) {}
  void SetID(int id) { id_ = id; }
  void SetName(std::string name) { name_ = name; }
  void SetDesc(std::string desc) { desc_ = desc; }

  int GetID() { return id_; }
  std::string GetName() { return name_; }
  std::string GetDesc() { return desc_; }
  std::vector<GroupUserEntity>& GetGroupUsers() { return group_users_; };

 private:
  int id_;
  std::string name_;
  std::string desc_;
  std::vector<GroupUserEntity> group_users_;
};

#endif