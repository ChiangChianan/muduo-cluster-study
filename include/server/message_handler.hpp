#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H
#include <muduo/base/Logging.h>
#include <muduo/net/TcpServer.h>
#include <functional>
#include <unordered_map>
#include "json.hpp"
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
  MsgHandlerFunc Dispatch(int msgid);

 private:
  MsgIDHandler();
  std::unordered_map<MsgType, MsgHandlerFunc> msg_handler_map_;
};

#endif