#ifndef CHATSERVER_H
#define CHATSERVER_H

#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"

class ChatServer {
 public:
  ChatServer(muduo::net::EventLoop* loop,
             const muduo::net::InetAddress& listenAddr,
             const std::string& nameArg);
  void start();

 private:
  muduo::net::TcpServer server_;
  muduo::net::EventLoop loop_;
};

#endif
