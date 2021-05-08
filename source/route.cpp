//
// Created by Pamula File on 5/8/21.
//

#include "route.hpp"

route::route( void ) { }

route::~route( void ) { }

route::route( route const &src ) {
    *this = src;
}

route &route::operator=( route const &src ) {
    ( void ) src;
    return *this;
}

bool route::check_name( const std::string &request ) {
    return (strncmp(_name.c_str(), request.c_str(), strlen(_name.c_str())));
}

std::string route::swap_path( const std::string &request ) {
    std::string tmp = request.substr(reinterpret_cast<unsigned long >(strlen(_name.c_str())), request.length());
    return tmp += _path;
}
