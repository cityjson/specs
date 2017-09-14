
project( citygml2cityjson )

cmake_minimum_required (VERSION 3.2)

add_definitions(-std=c++11)

set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true )
 
if ( COMMAND cmake_policy )
  cmake_policy( SET CMP0003 NEW )  
endif()


include_directories( ${GEOS_INCLUDE_DIR} )
include_directories( ${CMAKE_SOURCE_DIR}/thirdparty )

FILE(GLOB_RECURSE THIRDPARTY thirdparty/*.cpp)
add_library(thirdparty STATIC ${THIRDPARTY})

FILE(GLOB SRC_FILES src/*.cpp)
add_executable(citygml2cityjson ${SRC_FILES})

target_link_libraries(citygml2cityjson thirdparty)

install(TARGETS citygml2cityjson DESTINATION bin)
