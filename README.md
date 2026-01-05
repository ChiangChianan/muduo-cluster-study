# muduo-cluster-study

A chat server implementation based on the muduo network library, designed for learning and practicing modern C++ network programming techniques.

## Features
* Real-time messaging
* User authentication
* One-to-one chat
* Group chat
* Message persistence


## Requirements
### Operating System
* Linux (Ubuntu 20.04+ recommended)

### Dependencies
* muduo network library
* CMake 3.10+
* MySQL client library


## Quick Start

```bash
git clone https://github.com/ChiangChianan/muduo-cluster-study.git
cd muduo-cluster-study
sh build.sh
```

## Starting the Server

```bash
./build/bin/chatserver 127.0.0.1 6000
```

## Testing the Server

```bash
./build/bin/clientserver 127.0.0.1 6000
```



