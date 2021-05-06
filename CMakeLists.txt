cmake_minimum_required(VERSION 3.19)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserv serverf/main.cpp header.h source/error.cpp clientf/main.cpp)