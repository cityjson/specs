cmake_minimum_required (VERSION 3.2)
project (cjvalschema)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -Wno-unknown-pragmas")

set(${PROJECT_NAME}_VERSION_MAJOR 1)
set(${PROJECT_NAME}_VERSION_MINOR 0)


include_directories(include)
set(SOURCES src/main.cpp)

add_library(json-schema-validator SHARED
    include/json-schema-draft4.json.cpp
    include/json-uri.cpp
    include/jsoncpp.cpp
    include/json-validator.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} src/main.cpp)
target_link_libraries(${PROJECT_NAME} json-schema-validator)

