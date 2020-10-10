cmake_minimum_required(VERSION 2.8.3)
project(libuv_demo)

#SET(CMAKE_CXX_FLAGS "-Wall -std=c++11")


#set(ENV{PKG_CONFIG_PATH} /usr/local/lib/pkgconfig)
find_package(PkgConfig)
pkg_search_module(libuv REQUIRED libuv)  #/usr/local/lib/pkgconfig/libuv.pc

#find_package(libuv REQUIRED)
include_directories(${libuv_INCLUDE_DIRS} ./3rd) 

message("LIBUV_INCLUDE_DIRS:" ${libuv_INCLUDE_DIRS})
message("LIBUV_LIBRARIES:" ${libuv_LIBRARIES})


add_executable(main_api main_api.cpp)
target_link_libraries(main_api ${libuv_LIBRARIES})

add_executable(server server.c)
target_link_libraries(server ${libuv_LIBRARIES})

add_executable(client client.c)
target_link_libraries(client ${libuv_LIBRARIES})