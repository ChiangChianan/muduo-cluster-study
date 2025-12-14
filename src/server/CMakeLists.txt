add_executable(chatserver main.cpp chatserver.cpp)

target_include_directories(chatserver PUBLIC ${PROJECT_SOURCE_DIR}/include/server ${MUDUO_INCLUDE_DIR})
target_link_directories(chatserver PUBLIC ${MUDUO_INCLUDE_DIR}/build/lib)
target_link_libraries(chatserver PUBLIC muduo_net muduo_base Threads::Threads)
