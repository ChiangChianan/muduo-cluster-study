#!/bin/bash

set -e
echo "构建 Muduo 测试服务器..."
mkdir -p build && cd build
cmake ..
make
echo "构建完成"
echo "可执行文件: $(pwd)/bin/muduo_server"
