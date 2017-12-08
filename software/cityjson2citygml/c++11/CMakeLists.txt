cmake_minimum_required (VERSION 3.2)
project (cityjson2citygml)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

set(${PROJECT_NAME}_VERSION_MAJOR 1)
set(${PROJECT_NAME}_VERSION_MINOR 0)

# Boost
find_package( Boost 1.50.0 )
if ( NOT Boost_FOUND )
  message(SEND_ERROR "cjcompress requires the Boost library version 1.50 or higher")
  return()  
endif()
include_directories( ${Boost_INCLUDE_DIR} )

include_directories(include)
set(SOURCES src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

