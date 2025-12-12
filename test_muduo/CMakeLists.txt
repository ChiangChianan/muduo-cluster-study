cmake_minimum_required(VERSION 3.10)
project(server)

# 1. 查找系统依赖
find_package(Threads REQUIRED)

# 2. 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 3. 定义Muduo库的路径
set(MUDUO_ROOT ${CMAKE_SOURCE_DIR}/../thirdparty/muduo-master)
set(MUDUO_INCLUDE_DIR ${MUDUO_ROOT})

# 4. 创建可执行目标，并直接指定源文件
add_executable(muduo_server ${PROJECT_SOURCE_DIR}/test_muduo.cpp)

# 5. 为目标设置包含目录
target_include_directories(muduo_server PUBLIC ${MUDUO_INCLUDE_DIR})

# 6. 告诉链接器去哪里找库文件（替代直接写绝对路径）
target_link_directories(muduo_server PUBLIC ${MUDUO_ROOT}/build/lib)

# 7. 链接所需的库（注意：这里直接写库名，不需要路径和扩展名）
target_link_libraries(muduo_server PUBLIC muduo_net muduo_base Threads::Threads)