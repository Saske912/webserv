//
// Created by Pamula File on 5/8/21.
//

#include "t_config.hpp"

t_config::t_config( void ) { }

t_config::~t_config( void ) { }

t_config::t_config( t_config const &src ) {
    *this = src;
}

t_config &t_config::operator=( t_config const &src ) {
    this->_host = src._host;
    this->_port = src._port;
    return *this;
}

std::string t_config::get_host( ) {
    return _host;
}

void t_config::set_host( const std::string &host ) {
    _host = host;
}

unsigned int t_config::get_port( ) {
    return _port;
}

void t_config::set_port( unsigned int port ) {
    _port = port;
}

std::string t_config::get_server_name( ) {
    return _server_name;
}

void t_config::set_server_name( const std::string &server_name ) {
    _server_name = server_name;
}

t_config::t_config( const std::string  &host, unsigned int port,
     const std::list<std::string >& error_page, long int client_body_size) :
    _host(host), _port(port), _error_page(error_page), _client_body_size(client_body_size) {
}
