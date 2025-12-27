#include "chat_server.hpp"
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

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

void ChatServer::OnConnection(const muduo::net::TcpConnectionPtr& conn) {
  if (!conn->connected()) {
    MsgIDHandler::GetInstance()->ClientCloseException(conn);
    std::cout << "disconnected" << std::endl;
    conn->shutdown();
  }
}

void ChatServer::OnMessage(const muduo::net::TcpConnectionPtr& conn,
                           muduo::net::Buffer* buffer, muduo::Timestamp time) {
  std::string buff = buffer->retrieveAllAsString();
  json js = json::parse(buff);
  auto func = MsgIDHandler::GetInstance()->Dispatch(js["msgid"]);
  func(conn, js, time);
}