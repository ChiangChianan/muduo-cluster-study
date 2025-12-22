#include "signal_handler.hpp"
#include "message_handler.hpp"

std::mutex SignalHandler::signal_mutex_;
std::unordered_map<int, SignalHandler::HandlerFunc> SignalHandler::handlers_;

void SignalHandler::RegisterSignal(int signum, HandlerFunc handler) {
  std::lock_guard<std::mutex> lock(signal_mutex_);
  handlers_[signum] = std::move(handler);
  signal(signum, &SignalHandler::HandleSignal);
}

void SignalHandler::Init() {
  SignalHandler::RegisterSignal(SIGINT, &SignalHandler::HandlerReset);
  std::cout << "信号注册成功" << std::endl;
}

void SignalHandler::HandlerReset(int signum) {
  MsgIDHandler::GetInstance()->Reset();
  exit(0);
}