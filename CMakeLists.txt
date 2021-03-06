project(hse-sample-project)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
find_package(Boost 1.57.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(.)

add_library(redis
        logic/redis.cpp
        logic/reader.cpp
        logic/writer.cpp
        server/listener.cpp server/listener.h server/socket.cpp server/socket.h server/server.cpp server/server.h)
add_executable(redis-server
  main.cpp
)


target_link_libraries(redis-server redis pthread ${Boost_LIBRARIES})

include_directories(contrib)

add_executable(run_test
  contrib/gmock-gtest-all.cc
  contrib/gmock_main.cc
  test/redis_test.cpp
)
target_link_libraries(run_test redis pthread)
