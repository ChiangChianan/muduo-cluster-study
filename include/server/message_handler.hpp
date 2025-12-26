#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H
#include <muduo/base/Logging.h>
#include <muduo/net/TcpServer.h>
#include <functional>
#include <mutex>
#include <unordered_map>
#include "friend_dao.hpp"
#include "group_dao.hpp"
#include "json.hpp"
#include "offline_message_dao.hpp"
#include "redis.hpp"
#include "user_dao.hpp"
#include "utils.hpp"

using json = nlohmann::json;

using MsgHandlerFunc = std::function<void(const muduo::net::TcpConnectionPtr&,
                                          json, muduo::Timestamp)>;

class MsgIDHandler {
 public:
  static MsgIDHandler* GetInstance();
  void HandlerLogin(const muduo::net::TcpConnectionPtr& conn, json js,
                    muduo::Timestamp time);
  void HandlerRegister(const muduo::net::TcpConnectionPtr& conn, json js,
                       muduo::Timestamp time);
  void HandlerDirectChat(const muduo::net::TcpConnectionPtr& conn, json js,
                         muduo::Timestamp time);
  void HandlerAddFriend(const muduo::net::TcpConnectionPtr& conn, json js,
                        muduo::Timestamp time);
  void HandlerCreateGroup(const muduo::net::TcpConnectionPtr& conn, json js,
                          muduo::Timestamp time);
  void HandlerJoinGroup(const muduo::net::TcpConnectionPtr& conn, json js,
                        muduo::Timestamp time);
  void HandlerRedisSubscribeMessage(int user_id, std::string msg);
  void ClientCloseException(const muduo::net::TcpConnectionPtr& conn);
  void Reset();
  MsgHandlerFunc Dispatch(int msgid);

 private:
  MsgIDHandler();
  std::unordered_map<MsgType, MsgHandlerFunc> msg_handler_map_;
  // 存储在线用户的链接
  std::unordered_map<int, muduo::net::TcpConnectionPtr> user_conn_map_;
  std::mutex conn_mtx_;
  UserDAO user_dao_;
  OfflineMessageDAO offline_message_dao_;
  FriendDAO friend_dao_;
  GroupDAO group_dao_;
  RedisConn redis_conn_;
};

#endif