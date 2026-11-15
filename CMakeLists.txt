cmake_minimum_required(VERSION 3.10)
project(ros_sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

add_executable(ros_sim
    src/main.cpp
    src/ros_core.cpp
    src/publisher.cpp
    src/subscriber.cpp
    src/logger.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(ros_sim Threads::Threads)
