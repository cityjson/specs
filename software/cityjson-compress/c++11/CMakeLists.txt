cmake_minimum_required (VERSION 3.2)
project (cjcompress)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

set(${PROJECT_NAME}_VERSION_MAJOR 1)
set(${PROJECT_NAME}_VERSION_MINOR 0)

include_directories( ${CMAKE_SOURCE_DIR}/include/ )

# Boost
find_package( Boost 1.50.0 REQUIRED COMPONENTS program_options)
if ( NOT Boost_FOUND )
  message(SEND_ERROR "cjcompress requires the Boost library version 1.50 or higher")
  return()  
endif()
include_directories( ${Boost_INCLUDE_DIR} )

FILE(GLOB SRC_FILES src/*.cpp)
add_executable(cjcompress ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
