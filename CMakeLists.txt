cmake_minimum_required(VERSION 3.17.5)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(webserv source source/config .)

#add_link_options(-stdlib=libstdc++ -std=c++98 -L/usr/lib)

add_compile_options(-g -Wall -Wextra)

add_executable(webserv
        source/erase.cpp
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
        source/Number.cpp
        source/URI.cpp
        source/config/parse.cpp
        source/config/Token.cpp
        source/config/Lexer.cpp
        source/config/Position.cpp
        source/config/Parser.cpp
        source/config/ParseResult.cpp
        source/config/Interpreter.cpp
        source/config/Validator.cpp
        source/config/node/ANode.cpp
        source/config/node/ConfigNode.cpp
        source/config/node/IdentifierNode.cpp
        source/config/node/ParamNode.cpp
        source/config/node/RouteNode.cpp
        source/config/node/ServerNode.cpp
        source/config/node/ErrorNode.cpp
        source/header.cpp
        source/header.hpp
        source/trim.cpp
        source/template.hpp
        source/templates.cpp
        source/ft_doublefree.cpp
        source/get_next_line.cpp
        source/get_next_line_utils.cpp
        source/cgi_path.cpp source/datetime.cpp
        source/send_protected.cpp source/file_available.cpp
        source/get_http_line.cpp source/split_buffer.cpp
        source/parse_request.cpp
        source/templates.cpp)
