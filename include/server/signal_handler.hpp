#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <signal.h>
#include <functional>
#include <mutex>
#include <unordered_map>

class SignalHandler {
 public:
  using HandlerFunc = std::function<void(int)>;

  /**
   * @brief 注册信号处理函数
   * @param signum 信号编号 (如 SIGSEGV, SIGINT)
   * @param handler 处理函数，接收int信号编号参数
   */
  static void RegisterSignal(int signum, HandlerFunc handler);
  static void HandlerReset(int signum);
  static void Init();

 private:
  SignalHandler() = delete;
  ~SignalHandler() = delete;

  static void HandleSignal(int signum) {
    std::lock_guard<std::mutex> lock(signal_mutex_);
    auto it = handlers_.find(signum);
    if (it != handlers_.end() && it->second) {
      it->second(signum);
    }
  }

  static std::mutex signal_mutex_;
  static std::unordered_map<int, HandlerFunc> handlers_;
};

#endif