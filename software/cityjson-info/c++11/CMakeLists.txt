cmake_minimum_required (VERSION 3.2)
project (cityjson-info)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

set(${PROJECT_NAME}_VERSION_MAJOR 1)
set(${PROJECT_NAME}_VERSION_MINOR 0)

include_directories(include)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME})

