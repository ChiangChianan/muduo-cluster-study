#ifndef REDIS_H
#define REDIS_H

#include <functional>
#include <memory>
#include "hiredis/hiredis.h"

class RedisConn {
 public:
  RedisConn();
  ~RedisConn();

  RedisConn(const RedisConn&) = delete;
  RedisConn& operator=(const RedisConn&) = delete;

  RedisConn(RedisConn&& other) noexcept;
  RedisConn& operator=(RedisConn&& other) noexcept;

  struct RedisReplyDeleter {
    void operator()(redisReply* reply) const {
      if (reply) freeReplyObject(reply);
    }
  };
  using RedisReplyPtr = std::unique_ptr<redisReply, RedisReplyDeleter>;

  bool Connect();
  bool Publish(int channel, std::string message);
  bool Subscribe(int channel);
  bool Unsubscribe(int channel);
  void StartObserving();
  void InitNotifyHandler(std::function<void(int, const std::string&)> handler);

 private:
  struct RedisContextDeleter {
    void operator()(redisContext* context) const {
      if (context) redisFree(context);
    }
  };
  using RedisContextPtr = std::unique_ptr<redisContext, RedisContextDeleter>;
  RedisContextPtr publish_context_;
  RedisContextPtr subscribe_context_;
  std::function<void(int, const std::string&)> notify_message_handler_;
  bool should_stop_observing_;
};

#endif