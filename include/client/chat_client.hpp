#ifndef CLIENT_H
#define CLIENT_H

#include <semaphore.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include "arpa/inet.h"
#include "sys/socket.h"

class ChatClient {
 public:
  ChatClient() : clientfd_(-1) {
    memset(&server_addr_, 0, sizeof(server_addr_));
  }
  ~ChatClient() { Disconnect(); }

  bool ConnectToServer(const std::string& ip, int port) {
    // 创建socket
    clientfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd_ < 0) {
      std::cerr << "创建socket失败: " << strerror(errno) << std::endl;
      return false;
    }

    // 设置服务器地址信息
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(port);

    // 将IP地址从字符串转换为网络地址
    if (inet_pton(AF_INET, ip.c_str(), &server_addr_.sin_addr) <= 0) {
      std::cerr << "无效的IP地址: " << ip << std::endl;
      close(clientfd_);
      clientfd_ = -1;
      return false;
    }

    // 连接到服务器
    if (connect(clientfd_, (struct sockaddr*)&server_addr_,
                sizeof(server_addr_)) < 0) {
      std::cerr << "连接服务器失败: " << strerror(errno) << std::endl;
      close(clientfd_);
      clientfd_ = -1;
      return false;
    }

    std::cout << "成功连接到服务器 " << ip << ":" << port << std::endl;
    return true;
  }

  void Disconnect() {
    if (clientfd_ >= 0) {
      close(clientfd_);
      clientfd_ = -1;
      std::cout << "已断开连接" << std::endl;
    }
  }

  // 发送数据
  bool SendData(const std::string& message) {
    if (clientfd_ < 0) {
      std::cerr << "未连接到服务器" << std::endl;
      return false;
    }

    ssize_t bytes_sent = send(clientfd_, message.c_str(), message.length(), 0);
    if (bytes_sent < 0) {
      std::cerr << "发送数据失败: " << strerror(errno) << std::endl;
      return false;
    }

    std::cout << "已发送 " << bytes_sent << " 字节数据: " << message
              << std::endl;
    return true;
  }

  // 接收数据（指定缓冲区大小）
  std::string ReceiveData(size_t buffer_size = 1024) {
    if (clientfd_ < 0) {
      return "";
    }

    char buffer[buffer_size];
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytes_received = recv(clientfd_, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received < 0) {
      std::cerr << "接收数据失败: " << strerror(errno) << std::endl;
      return "";
    } else if (bytes_received == 0) {
      std::cout << "服务器断开连接" << std::endl;
      return "";
    }

    std::cout << "已接收 " << bytes_received << " 字节数据" << std::endl;
    return std::string(buffer, bytes_received);
  }

  int clientfd() { return clientfd_; }

  bool IsConnected() const { return clientfd_ >= 0; }

 private:
  int clientfd_;
  struct sockaddr_in server_addr_;
};

#endif