# Install script for directory: /home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/examples

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/ace/ttcp/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/asio/chat/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/asio/tutorial/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/fastcgi/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/filetransfer/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/hub/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/idleconnection/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/maxconnection/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/memcached/client/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/memcached/server/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/multiplexer/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/netty/discard/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/netty/echo/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/netty/uptime/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/pingpong/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/roundtrip/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/shorturl/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/simple/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/socks4a/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/sudoku/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/twisted/finger/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/wordcount/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/zeromq/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/curl/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/ace/logging/cmake_install.cmake")
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/examples/protobuf/cmake_install.cmake")

endif()

