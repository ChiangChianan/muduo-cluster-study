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
  msg_handler_map_.insert(
      {MsgType::kMsgDirectChat,
       std::bind(&MsgIDHandler::HandlerDirectChat, this, std::placeholders::_1,
                 std::placeholders::_2, std::placeholders::_3)});
  msg_handler_map_.insert(
      {MsgType::kMsgAddFriend,
       std::bind(&MsgIDHandler::HandlerAddFriend, this, std::placeholders::_1,
                 std::placeholders::_2, std::placeholders::_3)});
  msg_handler_map_.insert(
      {MsgType::kMsgCreateGroup,
       std::bind(&MsgIDHandler::HandlerCreateGroup, this, std::placeholders::_1,
                 std::placeholders::_2, std::placeholders::_3)});
  msg_handler_map_.insert(
      {MsgType::kMsgJoinGroup,
       std::bind(&MsgIDHandler::HandlerJoinGroup, this, std::placeholders::_1,
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

  UserEntity user = user_dao_.Query(id);
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
      user_dao_.UpdateState(user);

      json response;
      response["msgid"] = MsgType::kMsgLogACK;
      response["errno"] = 0;
      response["id"] = user.GetID();
      response["name"] = user.GetName();
      std::vector<std::string> offline_message =
          offline_message_dao_.QueryOfflineMessages(id);
      if (!offline_message.empty()) {
        response["offlinemsg"] = offline_message;
        offline_message_dao_.DeleteOfflineMessageByUserId(id);
      }
      std::vector<UserEntity> friends = friend_dao_.GetFriends(id);
      if (!friends.empty()) {
        std::vector<std::string> friends_info;
        for (UserEntity& user : friends) {
          json js;
          js["id"] = user.GetID();
          js["name"] = user.GetName();
          js["state"] = user.GetState();
          friends_info.push_back(js.dump());
        }
        response["friendinfo"] = friends_info;
      }

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
  bool state = user_dao_.Insert(user);
  if (state) {
    json response;
    response["msgid"] = MsgType::kMsgRegACK;
    response["errno"] = 0;
    response["id"] = user.GetID();
    conn->send(response.dump());
    LOG_INFO << "Registration successful";
  } else {
    json response;
    response["msgid"] = MsgType::kMsgRegACK;
    response["errno"] = 1;
    conn->send(response.dump());
    LOG_INFO << "Registration failed";
  }
}

void MsgIDHandler::HandlerDirectChat(const muduo::net::TcpConnectionPtr& conn,
                                     json js, muduo::Timestamp time) {
  int to_id = js["toid"].get<int>();
  {
    std::lock_guard<std::mutex> lock(conn_mtx_);
    auto it = user_conn_map_.find(to_id);
    if (it != user_conn_map_.end()) {
      it->second->send(js.dump());
      return;
    }
  }
  offline_message_dao_.StoreOfflineMessage(to_id, js.dump());
  LOG_INFO << "Store Offline Message";
}
void MsgIDHandler::HandlerAddFriend(const muduo::net::TcpConnectionPtr& conn,
                                    json js, muduo::Timestamp time) {
  int user_id = js["userid"].get<int>();
  int friend_id = js["friendid"].get<int>();
  friend_dao_.AddFriend(user_id, friend_id);
  LOG_INFO << "Add friend successful";
}
void MsgIDHandler::HandlerCreateGroup(const muduo::net::TcpConnectionPtr& conn,
                                      json js, muduo::Timestamp time) {
  int user_id = js["id"].get<int>();
  std::string group_name = js["groupname"];
  std::string group_desc = js["groupdesc"];
  GroupEntity group(-1, group_name, group_desc);
  if (group_dao_.CreateGroup(group)) {
    group_dao_.JoinGroup(user_id, group.GetID(), "creator");
  }
}

void MsgIDHandler::HandlerJoinGroup(const muduo::net::TcpConnectionPtr& conn,
                                    json js, muduo::Timestamp time) {
  int user_id = js["id"].get<int>();
  int group_id = js["group_id"].get<int>();
  group_dao_.JoinGroup(user_id, group_id, "normal");
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
    user_dao_.UpdateState(user);
  }
}

void MsgIDHandler::Reset() { user_dao_.ResetState(); }
/*
登录
{"msgid":1, "id":1, "password":"asd"}
{"msgid":1, "id":2, "password":"asdf"}
注册
{"msgid":3, "name":"xiaodeng", "password":"asdf"}
聊天
{"msgid":5, "toid":1, "fromname":"zhangdeng", "msg":"hello sir"}
加好友
{"msgid":6, "userid":1, "friendid":2}
创群
{"msgid":7, "id":1, "groupname":"liaotianqun1", "groupdesc":"chat group 1"}
加群
{"msgid":8, "id":2, "group_id": 5}
*/
