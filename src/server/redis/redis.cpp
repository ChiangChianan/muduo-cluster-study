#include "redis.hpp"
#include <iostream>
#include <thread>

RedisConn::RedisConn()
    : publish_context_(nullptr),
      subscribe_context_(nullptr),
      should_stop_observing_(true) {}
RedisConn::~RedisConn() {}

bool RedisConn::Connect() {
  publish_context_.reset(redisConnect("127.0.0.1", 6379));
  if (publish_context_ == nullptr) {
    std::cerr << "connect redis failed" << std::endl;
  }
  subscribe_context_.reset(redisConnect("127.0.0.1", 6379));
  if (subscribe_context_ == nullptr) {
    std::cerr << "connect redis failed" << std::endl;
  }
  std::thread t([&]() { StartObserving(); });
  t.detach();
  std::cout << "connect redis successful" << std::endl;
  return true;
}

bool RedisConn::Publish(int channel, std::string message) {
  if (!publish_context_) {
    std::cerr << "publish context is null" << std::endl;
    return false;
  }
  RedisReplyPtr reply_ptr((redisReply *)redisCommand(
      publish_context_.get(), "PUBLISH %d %s", channel, message.c_str()));
  if (reply_ptr == nullptr) {
    std::cerr << "publish command failed" << std::endl;
    return false;
  }
  return true;
}

bool RedisConn::Subscribe(int channel) {
  if (!subscribe_context_) {
    std::cerr << "subscribe context is null" << std::endl;
    return false;
  }
  if (REDIS_OK !=
      redisAppendCommand(subscribe_context_.get(), "SUBSCRIBE %d", channel)) {
    std::cerr << "subscribe command failed" << std::endl;
    return false;
  }
  int done = 0;
  if (REDIS_OK != redisBufferWrite(subscribe_context_.get(), &done)) {
    std::cerr << "subscribe command failed" << std::endl;
    return false;
  }
  return true;
}

bool RedisConn::Unsubscribe(int channel) {
  if (REDIS_OK !=
      redisAppendCommand(subscribe_context_.get(), "UNSUBSCRIBE %d", channel)) {
    std::cerr << "unsubscribe command failed" << std::endl;
    return false;
  }
  int done = 0;
  if (REDIS_OK != redisBufferWrite(subscribe_context_.get(), &done)) {
    std::cerr << "unsubscribe command failed" << std::endl;
    return false;
  }
  return true;
}

void RedisConn::StartObserving() {
  should_stop_observing_ = false;
  while (!should_stop_observing_) {
    redisReply *raw_reply = nullptr;
    if (REDIS_OK !=
        redisGetReply(subscribe_context_.get(), (void **)&raw_reply)) {
      std::cerr << "observing failed" << std::endl;
      break;
    }
    RedisReplyPtr reply_ptr(raw_reply);
    if (reply_ptr != nullptr && reply_ptr->element[2] != nullptr &&
        reply_ptr->element[2]->str != nullptr) {
      int channel = atoi(reply_ptr->element[1]->str);
      std::string message(reply_ptr->element[2]->str,
                          reply_ptr->element[2]->len);
      notify_message_handler_(channel, message);
    }
  }
  std::cerr << "observing quit" << std::endl;
}

void RedisConn::InitNotifyHandler(
    std::function<void(int, const std::string &)> fun) {
  notify_message_handler_ = fun;
}
