cmake_minimum_required (VERSION 3.2)
project (cjcompress)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

set(${PROJECT_NAME}_VERSION_MAJOR 1)
set(${PROJECT_NAME}_VERSION_MINOR 0)

include_directories( ${CMAKE_SOURCE_DIR}/thirdparty/ )

FILE(GLOB SRC_FILES src/*.cpp)
add_executable(cjcompress ${SRC_FILES})

target_link_libraries(${PROJECT_NAME})
