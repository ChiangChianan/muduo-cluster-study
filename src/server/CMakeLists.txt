add_executable(chatserver 
               main.cpp 
               chat_server.cpp
               message_handler.cpp
               user_model.cpp 
               database/database.cpp)

target_include_directories(chatserver PUBLIC ${PROJECT_SOURCE_DIR}/include/server 
                                             ${PROJECT_SOURCE_DIR}/include/common
                                             ${PROJECT_SOURCE_DIR}/include/server/database
                                             ${MUDUO_INCLUDE_DIR})
target_link_directories(chatserver PUBLIC ${MUDUO_INCLUDE_DIR}/build/lib)
target_link_libraries(chatserver PUBLIC muduo_net muduo_base mysqlclient Threads::Threads)
