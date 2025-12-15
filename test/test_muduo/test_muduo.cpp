#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>
#include <functional>
#include <iostream>

/**
 * @brief 基于muduo的聊天服务器类
 *
 * 关键实现步骤：
 * 1.组合TcpServer对象
 * 2.创建EventLoop事件循环对象的指针
 * 3.明确TcpServer构造函数需要什么函数，输出ChatServer的构造函数
 * 4.在当前服务类的构造函数中，注册处理连接的回调函数和注册处理读写事件的回调函数
 * 5.设置合适的服务端线程数量
 */
class ChatServer {
 public:
  ChatServer(muduo::net::EventLoop* loop,                // 事件循环
             const muduo::net::InetAddress& listenAddr,  // 服务器的IP+Port
             const std::string& nameArg)                 // 服务器的名字
      : _server(loop, listenAddr, nameArg), _loop(loop) {
    // 给服务器注册用户连接的创建和回调
    _server.setConnectionCallback(
        std::bind(&ChatServer::onConnection, this, std::placeholders::_1));
    // 给服务器注册用户读写事件回调
    _server.setMessageCallback(
        std::bind(&ChatServer::onMessage, this, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3));
    // 设置服务器端的线程数量 4个I/O处理线程
    _server.setThreadNum(4);
    // 开启事件循环
  }
  void start() { _server.start(); }

 private:
  // 专门处理用户的连接和断开
  void onConnection(const muduo::net::TcpConnectionPtr& conn) {
    if (conn->connected()) {
      std::cout << "IP:" << conn->peerAddress().toIp() << ' '
                << "Port:" << conn->peerAddress().toIpPort() << " to "
                << "IP:" << conn->localAddress().toIp() << ' '
                << "Port:" << conn->localAddress().toIpPort() << " is online"
                << std::endl;
    } else {
      std::cout << "IP:" << conn->peerAddress().toIp() << ' '
                << "Port:" << conn->peerAddress().toIpPort() << " to "
                << "IP:" << conn->localAddress().toIp() << ' '
                << "Port:" << conn->localAddress().toIpPort() << " is offline"
                << std::endl;
      conn->shutdown();
      // _loop->quit();
    }
  }
  // 专门处理用户的读写事件
  void onMessage(const muduo::net::TcpConnectionPtr& conn,  // 连接
                 muduo::net::Buffer* buffer,                // 缓冲区
                 muduo::Timestamp time) {  // 接收到数据的时间信息
    std::string buf = buffer->retrieveAllAsString();
    std::cout << "recv data:" << buf << "time:" << time.toString() << std::endl;
    conn->send(buf);
  }
  muduo::net::TcpServer _server;
  muduo::net::EventLoop* _loop;
};

int main() {
  muduo::net::EventLoop loop;
  muduo::net::InetAddress addr("127.0.0.1", 6000);
  ChatServer chat_server(&loop, addr, "Chat Server!");
  chat_server.start();
  loop.loop();
  return 0;
}
