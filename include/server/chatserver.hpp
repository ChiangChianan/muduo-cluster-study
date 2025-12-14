#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <functional>
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"

class ChatServer {
 public:
  ChatServer(muduo::net::EventLoop* loop,
             const muduo::net::InetAddress& listenAddr,
             const std::string& nameArg);
  void Start();

 private:
  void OnConnection(const muduo::net::TcpConnectionPtr&);
  void OnMessage(const muduo::net::TcpConnectionPtr&, muduo::net::Buffer*,
                 muduo::Timestamp);
  muduo::net::TcpServer server_;
  muduo::net::EventLoop* loop_;
};

#endif
