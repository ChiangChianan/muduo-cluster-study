# Install script for directory: /home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base

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

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/lib/libmuduo_base.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/muduo/base" TYPE FILE FILES
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/AsyncLogging.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/Atomic.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/BlockingQueue.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/BoundedBlockingQueue.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/Condition.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/CountDownLatch.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/CurrentThread.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/Date.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/Exception.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/FileUtil.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/GzipFile.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/LogFile.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/LogStream.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/Logging.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/Mutex.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/ProcessInfo.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/Singleton.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/StringPiece.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/Thread.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/ThreadLocal.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/ThreadLocalSingleton.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/ThreadPool.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/TimeZone.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/Timestamp.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/Types.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/WeakCallback.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/copyable.h"
    "/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/muduo/base/noncopyable.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/kinetic/project/muduo-cluster-server/thirdparty/muduo-master/build/muduo/base/tests/cmake_install.cmake")

endif()

