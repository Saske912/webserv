//
// Created by Pamula File on 5/8/21.
//
#include <iostream>
#include "route.hpp"

route::route( const std::string &name, const std::string &root, const std::list<std::string> &http_methods,
              bool autoindex, const std::string &def_page )
        : _name(name), _root(root), _http_methods(http_methods), _default_page(def_page), _autoindex(autoindex) {
}

route::route( void ) { }

route::~route( void ) { }

route::route( route const &src ) {
    *this = src;
}

route &route::operator=( route const &src ) {
    this->_root = src._root;
    this->_name = src._name;
    this->_http_methods = src._http_methods;
    this->_autoindex = src._autoindex;
    this->_default_page = src._default_page;
    return *this;
}

bool route::check_name( const std::string &request ) {
    return (strncmp(_name.c_str(), request.c_str(), strlen(_name.c_str())));
}

std::string route::swap_path( const std::string &request ) {
    std::string tmp = request.substr(reinterpret_cast<unsigned long >(strlen(_name.c_str())), request.length());
    return _root += tmp;
}

void route::set_default_page( const std::string &page ) {
    _default_page = page;
}

std::string route::get_default_page( ) const {
    return _default_page;
}

void route::autoindex_on( ) {
    _autoindex = true;
}

void route::autoindex_off( ) {
    _autoindex = false;
}

bool route::get_autoindex( ) const {
    return _autoindex;
}

std::list<std::string> route::get_http_methods( ) const {
    return _http_methods;
}

void route::set_http_methods( const std::list<std::string> &http_methods ) {
    _http_methods = http_methods;
}

std::string const &route::get_name( ) const {
    return _name;
}

std::string const &route::get_root( ) const {
    return _root;
}
