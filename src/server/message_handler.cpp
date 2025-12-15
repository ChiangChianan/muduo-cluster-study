#include "message_handler.hpp"

MsgIDHandler* MsgIDHandler::GetInstance() {
  static MsgIDHandler msgid_handler;
  return &msgid_handler;
}

MsgIDHandler::MsgIDHandler() {
  msg_handler_map_.insert(
      {MsgType::kLogMsg,
       std::bind(&MsgIDHandler::HandlerLogin, this, std::placeholders::_1,
                 std::placeholders::_2, std::placeholders::_3)});
  msg_handler_map_.insert(
      {MsgType::kRegMsg,
       std::bind(&MsgIDHandler::HandlerRegister, this, std::placeholders::_1,
                 std::placeholders::_2, std::placeholders::_3)});
}

MsgHandlerFunc MsgIDHandler::Dispatch(int msgid) {
  MsgType msg_type = static_cast<MsgType>(msgid);
  auto it = msg_handler_map_.find(msg_type);
  if (it == msg_handler_map_.end()) {
    return [=](auto a, auto b, auto c) {
      LOG_ERROR << "msgid:" << msgid << " can not find handler";
    };
  }
  return msg_handler_map_[msg_type];
}

void MsgIDHandler::HandlerLogin(const muduo::net::TcpConnectionPtr& conn,
                                json js, muduo::Timestamp time) {
  LOG_INFO << "log successful";
}

void MsgIDHandler::HandlerRegister(const muduo::net::TcpConnectionPtr& conn,
                                   json js, muduo::Timestamp time) {
  LOG_INFO << "register successful";
}