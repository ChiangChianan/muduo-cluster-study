add_executable(clientserver main.cpp)

target_include_directories(clientserver PUBLIC ${PROJECT_SOURCE_DIR}/include/client 
                                               ${PROJECT_SOURCE_DIR}/include/server/model
                                               ${PROJECT_SOURCE_DIR}/include/common
                                               ${PROJECT_SOURCE_DIR}/include/client/redis)

target_link_libraries(clientserver PUBLIC Threads::Threads)