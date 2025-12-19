#include "message_handler.hpp"
#include "user_entity.hpp"

MsgIDHandler* MsgIDHandler::GetInstance() {
  static MsgIDHandler msgid_handler;
  return &msgid_handler;
}

MsgIDHandler::MsgIDHandler() {
  msg_handler_map_.insert(
      {MsgType::kMsgLog,
       std::bind(&MsgIDHandler::HandlerLogin, this, std::placeholders::_1,
                 std::placeholders::_2, std::placeholders::_3)});
  msg_handler_map_.insert(
      {MsgType::kMsgReg,
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
  int id = js["id"].get<int>();
  std::string pwd = js["password"];

  UserEntity user = user_model_.Query(id);
  if (user.GetID() == id && user.GetPassword() == pwd) {
    if (user.GetState() == "online") {
      json response;
      response["msgid"] = MsgType::kMsgLogACK;
      response["errno"] = 2;
      response["errmsg"] = "该账号已登录";
      conn->send(response.dump());
    } else {
      {
        std::lock_guard<std::mutex> lock(conn_mtx_);
        user_conn_map_.insert({id, conn});
      }
      user.SetState("online");
      user_model_.UpdateState(user);

      json response;
      response["msgid"] = MsgType::kMsgLogACK;
      response["errno"] = 0;
      response["id"] = user.GetID();
      response["name"] = user.GetName();
      conn->send(response.dump());
    }
  } else {
    json response;
    response["msgid"] = MsgType::kMsgLogACK;
    response["errno"] = 1;
    response["errmsg"] = "账号或密码错误";
    conn->send(response.dump());
  }
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

void MsgIDHandler::ClientCloseException(
    const muduo::net::TcpConnectionPtr& conn) {
  std::lock_guard<std::mutex> lock(conn_mtx_);
  UserEntity user;
  for (auto it = user_conn_map_.begin(); it != user_conn_map_.end(); ++it) {
    if (it->second == conn) {
      user.SetID(it->first);
      user_conn_map_.erase(it);
      break;
    }
  }
  if (user.GetID() != -1) {
    user.SetState("offline");
    user_model_.UpdateState(user);
  }
}
/*
{"msgid":1, "id":1, "password":"asd"}
*/