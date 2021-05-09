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

std::string t_config::get_host( ) const {
    return _host;
}

void t_config::set_host( const std::string &host ) {
    _host = host;
}

unsigned int t_config::get_port( ) const {
    return _port;
}

void t_config::set_port( unsigned int port ) {
    _port = port;
}

std::list<std::string> t_config::get_server_names( ) const {
    return _server_names;
}

void t_config::set_server_name( const std::list<std::string> &server_name ) {
    _server_names = server_name;
}

t_config::t_config( const std::string  &host, unsigned int port,
     const std::list<std::string >& error_page, std::list<route> const & routs, long int client_body_size) :
     _host(host), _port(port), _error_pages(error_page), _routs(routs), _client_body_size(client_body_size) {
}

std::string t_config::get_path_to_request( const std::string &request ) {
    std::list<route>::iterator it = _routs.begin();
    while (it != _routs.end())
    {
        if (!(*it).check_name(request))
            return (*it).swap_path(request);
        it++;
    }
    return NULL;
}

std::list<std::string> t_config::get_error_pages( ) const {
    return _error_pages;
}

void t_config::set_error_pages( const std::list<std::string> &err_pages ) {
    _error_pages = err_pages;
}

long int t_config::get_client_body_size( ) const {
    return _client_body_size;
}
