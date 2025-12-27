#ifndef UTILS_H
#define UTILS_H

enum class MsgType {
  kMsgLog = 1,     // 登录信息
  kMsgLogACK,      // 登录应答信息
  kMsgLogout,      // 注销消息
  kMsgReg,         // 注册信息
  kMsgRegACK,      // 注册应答信息
  kMsgDirectChat,  // 1v1聊天信息
  kMsgAddFriend,   // 添加好友请求

  kMsgCreateGroup,  // 创建群组
  kMsgJoinGroup,    // 加入群组
  kMsgGroupChat     // 群聊信息
};

#endif