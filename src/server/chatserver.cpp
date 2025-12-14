#include "chatserver.hpp"

ChatServer::ChatServer(muduo::net::EventLoop* loop,
                       const muduo::net::InetAddress& listenAddr,
                       const std::string& nameArg)
    : server_(loop, listenAddr, nameArg), loop_(loop) {
  server_.setConnectionCallback(
      std::bind(&ChatServer::OnConnection, this, std::placeholders::_1));
  server_.setMessageCallback(
      std::bind(&ChatServer::OnMessage, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3));
}

void ChatServer::Start() { server_.start(); }

void ChatServer::OnConnection(const muduo::net::TcpConnectionPtr&) {}

void ChatServer::OnMessage(const muduo::net::TcpConnectionPtr&,
                           muduo::net::Buffer*, muduo::Timestamp) {}