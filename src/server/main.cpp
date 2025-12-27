#include <iostream>
#include "chat_server.hpp"
#include "signal_handler.hpp"

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "command invalid! example:127.0.0.1 6000" << std::endl;
  }
  SignalHandler::Init();
  char* ip = argv[1];
  uint16_t port = atoi(argv[2]);
  muduo::net::EventLoop loop;
  muduo::net::InetAddress addr(ip, port);
  ChatServer chat_server(&loop, addr, "my_chat_server");

  chat_server.Start();
  loop.loop();

  return 0;
}