cmake_minimum_required(VERSION 3.17.5)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserv serverf/main.cpp header.h source/error.cpp source/init.cpp source/loop.cpp source/config.cpp source/t_config.cpp source/t_config.hpp)