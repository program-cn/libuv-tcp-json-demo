cmake_minimum_required(VERSION 2.8.3)
project(libuv_demo)

SET(CMAKE_CXX_FLAGS "-Wall -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/include  
                    ${PROJECT_SOURCE_DIR}/3rd/json
                    ${PROJECT_SOURCE_DIR}/3rd/libuv/include)

link_directories(${PROJECT_SOURCE_DIR}/3rd/libuv/lib)

find_package(glog)

## link static lib
# SET(CMAKE_CXX_COMPILER /usr/bin/clang++)
# add_executable(main_api main_api.cpp)
# target_link_libraries(main_api libuv_a.a pthread -ldl)


add_library(uv-cpp ./src/uv-cpp.cpp)

#### demo
add_executable(server ./demo/server.c)
target_link_libraries(server  -luv)

add_executable(client ./demo/client.c)
target_link_libraries(client  -luv)

## main-demo
add_executable(main_api ./src/main_api.cpp)
target_link_libraries(main_api -luv)

## main
add_executable(main ./src/main.cpp ./src/server.cpp)
target_link_libraries(main -luv glog uv-cpp)