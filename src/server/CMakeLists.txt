add_executable(chatserver 
               main.cpp 
               chat_server.cpp
               message_handler.cpp
               signal_handler.cpp
               model/user_dao.cpp 
               model/offline_message_dao.cpp
               model/friend_dao.cpp
               model/group_dao.cpp
               redis/redis.cpp
               database/database.cpp)

target_include_directories(chatserver PUBLIC ${PROJECT_SOURCE_DIR}/include/server 
                                             ${PROJECT_SOURCE_DIR}/include/common
                                             ${PROJECT_SOURCE_DIR}/include/server/database
                                             ${PROJECT_SOURCE_DIR}/include/server/model
                                             ${PROJECT_SOURCE_DIR}/include/server/redis
                                             ${MUDUO_INCLUDE_DIR})
target_link_directories(chatserver PUBLIC ${MUDUO_INCLUDE_DIR}/build/lib)
target_link_libraries(chatserver PUBLIC muduo_net muduo_base mysqlclient hiredis Threads::Threads)
