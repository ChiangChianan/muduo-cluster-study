#include <atomic>
#include <functional>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <vector>
#include "chat_client.hpp"
#include "group_entity.hpp"
#include "group_user_entity.hpp"
#include "json.hpp"
#include "user_entity.hpp"
#include "utils.hpp"

using json = nlohmann::json;

// 记录当前系统登录的用户信息
UserEntity g_current_user;
// 记录当前登录用户的好友列表信息
std::vector<UserEntity> g_current_user_friend_list;
// 记录当前登录用户的群组列表信息
std::vector<GroupEntity> g_current_user_group_list;

sem_t g_read_write_mutex;                        // 设置读写信号量
bool g_is_login_success;                         // 判断是否注册成功
std::atomic_bool g_is_main_menu_running{false};  // 控制主菜单页面程序

// 接收线程
void ReadTaskHandler(int clientfd);
// 主聊天页面程序
void MainMenu(int);
// 显示当前登录成功用户的基本信息
void ShowCurrentUserData();
// 获取系统时间（聊天信息需要添加时间信息）
std::string GetCurrentTime();

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "command invalid! example : 127.0.0.1 8000" << std::endl;
  }
  char *ip = argv[1];
  uint16_t port = atoi(argv[2]);
  ChatClient client;
  client.ConnectToServer(ip, port);

  // 初始化线程读写信号量
  sem_init(&g_read_write_mutex, 0, 0);

  // 连接服务器成功，启动接收子线程
  std::thread readTask(ReadTaskHandler, client.clientfd());  // pthread_create
  readTask.detach();                                         // pthread_detach

  while (1) {
    // 显示首页面菜单 登录、注册、退出
    std::cout << "========================" << std::endl;
    std::cout << "1. login" << std::endl;
    std::cout << "2. register" << std::endl;
    std::cout << "3. quit" << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << "choice:";
    int choice = 0;
    std::cin >> choice;
    std::cin.get();  // 读掉缓冲区残留的回车
    switch (choice) {
      case 1:  // login业务
      {
        int id = 0;
        char pwd[50] = {0};
        std::cout << "user id:";
        std::cin >> id;
        std::cin.get();  // 读掉缓冲区残留的回车
        std::cout << "user password:";
        std::cin.getline(pwd, 50);

        json js;
        js["msgid"] = MsgType::kMsgLog;
        js["id"] = id;
        js["password"] = pwd;

        std::string request = js.dump();
        g_is_login_success = false;

        int len = send(client.clientfd(), request.c_str(),
                       strlen(request.c_str()) + 1, 0);
        if (len == -1) {
          std::cerr << "send login msg error:" << request << std::endl;
        }

        sem_wait(
            &g_read_write_mutex);  // 等待信号量，由子线程处理完登录的响应消息后，通知这里

        if (g_is_login_success) {
          // 进入聊天主菜单页面
          g_is_main_menu_running = true;
          MainMenu(client.clientfd());
        }
      } break;
      case 2:  // register业务
      {
        char name[50] = {0};
        char pwd[50] = {0};
        std::cout << "username:";
        std::cin.getline(name, 50);
        std::cout << "userpassword:";
        std::cin.getline(pwd, 50);

        json js;
        js["msgid"] = MsgType::kMsgReg;
        js["name"] = name;
        js["password"] = pwd;
        std::string request = js.dump();

        int len = send(client.clientfd(), request.c_str(),
                       strlen(request.c_str()) + 1, 0);
        if (len == -1) {
          std::cerr << "send reg msg error:" << request << std::endl;
        }

        sem_wait(
            &g_read_write_mutex);  // 等待信号量，子线程处理完注册消息会通知
      } break;
      case 3:  // quit业务
        close(client.clientfd());
        sem_destroy(&g_read_write_mutex);
        exit(0);
      default:
        std::cerr << "invalid input!" << std::endl;
        break;
    }
  }

  return 0;
}

// 处理注册的响应逻辑
void HandleRegistrationResponse(json &responsejs) {
  if (0 != responsejs["errno"].get<int>())  // 注册失败
  {
    std::cerr << "name is already exist, register error!" << std::endl;
  } else  // 注册成功
  {
    std::cout << "name register success, userid is " << responsejs["id"]
              << ", do not forget it!" << std::endl;
  }
}

// 处理登录的响应逻辑
void HandleLoginResponse(json &responsejs) {
  if (0 != responsejs["errno"].get<int>())  // 登录失败
  {
    std::cerr << responsejs["errmsg"] << std::endl;
    g_is_login_success = false;
  } else  // 登录成功
  {
    // 记录当前用户的id和name
    int id = responsejs["id"].get<int>();
    std::string name = responsejs["name"];
    g_current_user.SetId(id);
    g_current_user.SetName(name);

    // 记录当前用户的好友列表信息
    if (responsejs.contains("friends")) {
      // 初始化
      g_current_user_friend_list.clear();

      std::vector<std::string> vec = responsejs["friends"];
      for (std::string &str : vec) {
        json js = json::parse(str);
        UserEntity user;
        user.SetId(js["id"].get<int>());
        user.SetName(js["name"]);
        user.SetState(js["state"]);
        g_current_user_friend_list.push_back(user);
      }
    }

    // 记录当前用户的群组列表信息
    if (responsejs.contains("groups")) {
      // 初始化
      g_current_user_group_list.clear();

      std::vector<std::string> vec1 = responsejs["groups"];
      for (std::string &groupstr : vec1) {
        json grpjs = json::parse(groupstr);
        GroupEntity group;
        group.SetId(grpjs["id"].get<int>());
        group.SetName(grpjs["groupname"]);
        group.SetDesc(grpjs["groupdesc"]);

        std::vector<std::string> vec2 = grpjs["users"];
        for (std::string &userstr : vec2) {
          GroupUserEntity user;
          json js = json::parse(userstr);
          user.SetId(js["id"].get<int>());
          user.SetName(js["name"]);
          user.SetState(js["state"]);
          user.SetRole(js["role"]);
          group.GetGroupUsers().push_back(user);
        }

        g_current_user_group_list.push_back(group);
      }
    }

    // 显示登录用户的基本信息
    ShowCurrentUserData();

    // 显示当前用户的离线消息  个人聊天信息或者群组消息
    if (responsejs.contains("offlinemsg")) {
      std::vector<std::string> vec = responsejs["offlinemsg"];
      for (std::string &str : vec) {
        json js = json::parse(str);
        // time + [id] + name + " said: " + xxx
        if (MsgType::kMsgDirectChat ==
            static_cast<MsgType>(js["msgid"].get<int>())) {
          std::cout << js["time"].get<std::string>() << " [" << js["id"] << "]"
                    << js["name"].get<std::string>()
                    << " said: " << js["msg"].get<std::string>() << std::endl;
        } else {
          std::cout << "群消息[" << js["groupid"]
                    << "]:" << js["time"].get<std::string>() << " [" << js["id"]
                    << "]" << js["name"].get<std::string>()
                    << " said: " << js["msg"].get<std::string>() << std::endl;
        }
      }
    }

    g_is_login_success = true;
  }
}

// 显示当前登录成功用户的基本信息
void ShowCurrentUserData() {
  std::cout << "======================login user======================"
            << std::endl;
  std::cout << "current login user => id:" << g_current_user.GetId()
            << " name:" << g_current_user.GetName() << std::endl;
  std::cout << "----------------------friend list---------------------"
            << std::endl;
  if (!g_current_user_friend_list.empty()) {
    for (UserEntity &user : g_current_user_friend_list) {
      std::cout << user.GetId() << " " << user.GetName() << " "
                << user.GetState() << std::endl;
    }
  }
  std::cout << "----------------------group list----------------------"
            << std::endl;
  if (!g_current_user_group_list.empty()) {
    for (GroupEntity &group : g_current_user_group_list) {
      std::cout << group.GetId() << " " << group.GetName() << " "
                << group.GetDesc() << std::endl;
      for (GroupUserEntity &user : group.GetGroupUsers()) {
        std::cout << user.GetId() << " " << user.GetName() << " "
                  << user.GetState() << " " << user.GetRole() << std::endl;
      }
    }
  }
  std::cout << "======================================================"
            << std::endl;
}

void ReadTaskHandler(int clientfd) {
  while (1) {
    /*
      这里必须要重新初始化
      第一次登录：收到 {"msgid":3,"name":"Alice"}（20字节）
      第二次登录：收到 {"msgid":3,"name":"Bob"}（18字节）
      buffer前18字节被覆盖
      buffer第19字节（'e'）和第20字节（'}'）仍然是上次的数据
      json解析器可能看到 {"msgid":3,"name":"Bob"}e}，解析失败
    */
    char buffer[1024] = {0};
    // 默认情况下 recv() 会阻塞
    ssize_t len = recv(clientfd, buffer, sizeof(buffer) - 1, 0);
    if (-1 == len || 0 == len) {
      close(clientfd);
      exit(-1);
    }
    buffer[len] = '\0';
    // 接收ChatServer转发的数据，反序列化生成json数据对象
    json js = json::parse(buffer);
    int msgid = js["msgid"].get<int>();
    MsgType msgtype = static_cast<MsgType>(msgid);

    if (MsgType::kMsgDirectChat == msgtype) {
      std::cout << js["time"].get<std::string>() << " [" << js["id"] << "]"
                << js["name"].get<std::string>()
                << " said: " << js["msg"].get<std::string>() << std::endl;
      continue;
    }

    if (MsgType::kMsgLogACK == msgtype) {
      HandleLoginResponse(js);        // 处理登录响应的业务逻辑
      sem_post(&g_read_write_mutex);  // 通知主线程，登录结果处理完成
      continue;
    }

    if (MsgType::kMsgRegACK == msgtype) {
      HandleRegistrationResponse(js);
      sem_post(&g_read_write_mutex);  // 通知主线程，注册结果处理完成
      continue;
    }
  }
}

// "help" command handler
void Help(int fd = 0, std::string str = "");
// "chat" command handler
void Chat(int, std::string);
// "addfriend" command handler
void AddFriend(int, std::string);
// "creategroup" command handler
void CreateGroup(int, std::string);
// "joingroup" command handler
void JoinGroup(int, std::string);
// "groupchat" command handler
void GroupChat(int, std::string);
// "logout" command handler
void Logout(int, std::string);

// 系统支持的客户端命令列表
std::unordered_map<std::string, std::string> command_descriptions = {
    {"help", "显示所有支持的命令，格式help"},
    {"chat", "一对一聊天，格式chat:friendid:message"},
    {"addfriend", "添加好友，格式addfriend:friendid"},
    {"creategroup", "创建群组，格式creategroup:groupname:groupdesc"},
    {"joingroup", "加入群组，格式joingroup:groupid"},
    {"groupchat", "群聊，格式groupchat:groupid:message"},
    {"logout", "注销，格式logout"}};

// 注册系统支持的客户端命令处理
std::unordered_map<std::string, std::function<void(int, std::string)>>
    command_handlers = {{"help", Help},           {"chat", Chat},
                        {"addfriend", AddFriend}, {"creategroup", CreateGroup},
                        {"joingroup", JoinGroup}, {"groupchat", GroupChat},
                        {"logout", Logout}};

void MainMenu(int clientfd) {
  Help();

  char buffer[1024] = {0};
  while (g_is_main_menu_running) {
    std::cin.getline(buffer, 1024);
    std::string commandbuf(buffer);
    std::string command;  // 存储命令
    int idx = commandbuf.find(":");
    if (-1 == idx) {
      command = commandbuf;
    } else {
      command = commandbuf.substr(0, idx);
    }
    auto it = command_handlers.find(command);
    if (it == command_handlers.end()) {
      std::cerr << "invalid input command!" << std::endl;
      continue;
    }

    // 调用相应命令的事件处理回调，MainMenu对修改封闭，添加新功能不需要修改该函数
    it->second(clientfd,
               commandbuf.substr(idx + 1,
                                 commandbuf.size() - idx));  // 调用命令处理方法
  }
}

// "help" command handler
void Help(int, std::string) {
  std::cout << "show command list >>> " << std::endl;
  for (auto &p : command_descriptions) {
    std::cout << p.first << " : " << p.second << std::endl;
  }
  std::cout << std::endl;
}

// "addfriend" command handler
void AddFriend(int clientfd, std::string str) {
  int friend_id = atoi(str.c_str());
  json js;
  js["msgid"] = MsgType::kMsgAddFriend;
  js["id"] = g_current_user.GetId();
  js["friendid"] = friend_id;
  std::string buffer = js.dump();
  int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
  if (-1 == len) {
    std::cerr << "send addfriend msg error -> " << buffer << std::endl;
  }
}

// "chat" command handler
void Chat(int clientfd, std::string str) {
  int idx = str.find(":");  // friendid:message
  if (-1 == idx) {
    std::cerr << "chat command invalid!" << std::endl;
    return;
  }

  int friendid = atoi(str.substr(0, idx).c_str());
  std::string message = str.substr(idx + 1, str.size() - idx);

  json js;
  js["msgid"] = MsgType::kMsgDirectChat;
  js["id"] = g_current_user.GetId();
  js["name"] = g_current_user.GetName();
  js["toid"] = friendid;
  js["msg"] = message;
  js["time"] = GetCurrentTime();
  std::string buffer = js.dump();

  int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
  if (-1 == len) {
    std::cerr << "send chat msg error -> " << buffer << std::endl;
  }
}

// "creategroup" command handler  groupname:groupdesc
void CreateGroup(int clientfd, std::string str) {
  int idx = str.find(":");
  if (-1 == idx) {
    std::cerr << "creategroup command invalid!" << std::endl;
    return;
  }

  std::string groupname = str.substr(0, idx);
  std::string groupdesc = str.substr(idx + 1, str.size() - idx);

  json js;
  js["msgid"] = MsgType::kMsgCreateGroup;
  js["id"] = g_current_user.GetId();
  js["groupname"] = groupname;
  js["groupdesc"] = groupdesc;
  std::string buffer = js.dump();

  int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
  if (-1 == len) {
    std::cerr << "send creategroup msg error -> " << buffer << std::endl;
  }
}

// "joingroup" command handler
void JoinGroup(int clientfd, std::string str) {
  int groupid = atoi(str.c_str());
  json js;
  js["msgid"] = MsgType::kMsgJoinGroup;
  js["id"] = g_current_user.GetId();
  js["groupid"] = groupid;
  std::string buffer = js.dump();

  int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
  if (-1 == len) {
    std::cerr << "send joingroup msg error -> " << buffer << std::endl;
  }
}

// "groupchat" command handler   groupid:message
void GroupChat(int clientfd, std::string str) {
  int idx = str.find(":");
  if (-1 == idx) {
    std::cerr << "groupchat command invalid!" << std::endl;
    return;
  }

  int groupid = atoi(str.substr(0, idx).c_str());
  std::string message = str.substr(idx + 1, str.size() - idx);

  json js;
  js["msgid"] = MsgType::kMsgGroupChat;
  js["id"] = g_current_user.GetId();
  js["name"] = g_current_user.GetName();
  js["groupid"] = groupid;
  js["msg"] = message;
  js["time"] = GetCurrentTime();
  std::string buffer = js.dump();

  int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
  if (-1 == len) {
    std::cerr << "send groupchat msg error -> " << buffer << std::endl;
  }
}

// "logout" command handler
void Logout(int clientfd, std::string) {
  json js;
  js["msgid"] = MsgType::kMsgLogout;
  js["id"] = g_current_user.GetId();
  std::string buffer = js.dump();

  int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
  if (-1 == len) {
    std::cerr << "send logout msg error -> " << buffer << std::endl;
  } else {
    g_is_main_menu_running = false;
  }
}

// 获取系统时间（聊天信息需要添加时间信息）
std::string GetCurrentTime() {
  auto tt =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  struct tm *ptm = localtime(&tt);
  char date[60] = {0};
  sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", (int)ptm->tm_year + 1900,
          (int)ptm->tm_mon + 1, (int)ptm->tm_mday, (int)ptm->tm_hour,
          (int)ptm->tm_min, (int)ptm->tm_sec);
  return std::string(date);
}