#include <iostream>
#include "chat_server.hpp"

int main() {
  muduo::net::EventLoop loop;
  muduo::net::InetAddress addr("127.0.0.1", 6000);
  ChatServer chat_server(&loop, addr, "my_chat_server");

  chat_server.Start();
  loop.loop();

  return 0;
}