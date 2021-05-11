cmake_minimum_required(VERSION 3.17.5)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(webserv source source/config .)

add_link_options(-L../libft -lft)

add_executable(webserv
        header.h
        itressa.h
        wpersimm.h
        pfile.h
        serverf/main.cpp
        source/error.cpp
        source/init.cpp
        source/loop.cpp
        source/default_config.cpp
        source/server.cpp
        source/server.hpp
        source/route.cpp
        source/route.hpp
        source/tests.cpp
        source/config.cpp
        source/config.hpp
        source/config/parse.cpp
        source/config/Token.cpp
        source/config/Lexer.cpp
        source/config/Position.cpp
        source/config/Node.cpp
        source/config/Parser.cpp
        source/config/ParseResult.cpp
        source/config/Interpreter.cpp
        )
