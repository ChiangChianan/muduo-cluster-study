#ifndef OFFLINE_MESSAGE_DAO_H
#define OFFLINE_MESSAGE_DAO_H
#include <iostream>
#include <vector>
class OfflineMessageDAO {
 public:
  // 存储离线消息
  void StoreOfflineMessage(int user_id, const std::string& message);

  // 删除指定用户的离线消息
  void DeleteOfflineMessageByUserId(int user_id);

  // 查询离线消息
  std::vector<std::string> QueryOfflineMessages(int user_id);
};
#endif