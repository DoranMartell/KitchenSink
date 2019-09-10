cmake_minimum_required(VERSION 2.8)
project(final)
add_executable(final main.c serv.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")