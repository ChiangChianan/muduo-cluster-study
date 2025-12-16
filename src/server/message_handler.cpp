#include "message_handler.hpp"
#include "user_entity.hpp"

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
  LOG_INFO << "Login successful";
}

void MsgIDHandler::HandlerRegister(const muduo::net::TcpConnectionPtr& conn,
                                   json js, muduo::Timestamp time) {
  std::string name = js["name"];
  std::string pwd = js["password"];
  UserEntity user;
  user.SetName(name);
  user.SetPassword(pwd);
  bool state = user_model_.Insert(user);
  if (state) {
    json response;
    response["msgid"] = MsgType::kMsgACK;
    response["errno"] = 0;
    response["id"] = user.GetID();
    conn->send(response.dump());
    LOG_INFO << "Registration successful";
  } else {
    json response;
    response["msgid"] = MsgType::kMsgACK;
    response["errno"] = 1;
    conn->send(response.dump());
    LOG_INFO << "Registration failed";
  }
}
