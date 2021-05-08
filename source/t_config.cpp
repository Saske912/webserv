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

std::list<std::string> t_config::get_server_name( ) {
    return _server_names;
}

void t_config::set_server_name( const std::list<std::string> &server_name ) {
    _server_names = server_name;
}

t_config::t_config( const std::string  &host, unsigned int port,
     const std::list<std::string >& error_page, long int client_body_size) :
    _host(host), _port(port), _error_pages(error_page), _client_body_size(client_body_size) {
}

std::string t_config::get_path_to_request( const std::string &request ) {
    std::list<route>::iterator it = _rout.begin();
    while (it != _rout.end())
    {
        if (!(*it).check_name(request))
            return (*it).swap_path(request);
        it++;
    }
    return NULL;
}
