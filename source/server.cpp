//
// Created by Pamula File on 5/8/21.
//

#include "server.hpp"

server::server() : _server_names(), _error_pages(), _routs() {}

server::~server( void ) { }

server::server( server const &src ) {
    *this = src;
}

server &server::operator=( server const &src ) {
    this->_host = src._host;
    this->_port = src._port;
    this->_routs = src._routs;
    this->_client_body_size = src._client_body_size;
    this->_error_pages = src._error_pages;
    this->_server_names = src._server_names;
    return *this;
}

std::string server::get_host( ) const {
    return _host;
}

void server::set_host( const std::string &host ) {
    _host = host;
}

unsigned int server::get_port( ) const {
    return _port;
}

void server::set_port( unsigned int port ) {
    _port = port;
}

const std::list<std::string>& server::get_server_names( ) const {
    return _server_names;
}

void server::set_server_names( const std::list<std::string> &server_name ) {
    _server_names = server_name;
}

void server::add_server_name(const std::string &server_name)
{
	_server_names.push_back(std::string(server_name));
}

server::server( const std::string  &host, unsigned int port,
     const std::map<int, std::string >& error_pages, std::list<route> const & routs, long int client_body_size) :
     _host(host), _port(port), _error_pages(error_pages), _routs(routs), _client_body_size(client_body_size) {
}

std::string server::get_path_to_request( const std::string &request ) {
    std::list<route>::iterator it = _routs.begin();
    while (it != _routs.end())
    {
        if (!(*it).check_name(request))
        {
            return request_processing((*it).swap_path(request), (*it).get_default_page());
        }
        it++;
    }
    throw std::exception();
}

const std::map<int, std::string>& server::get_error_pages( ) const {
    return _error_pages;
}

void server::set_error_pages( const std::map<int, std::string> &err_pages ) {
    _error_pages = err_pages;
}

void server::add_error_page(int error_code, const std::string &filename)
{
	_error_pages.insert(std::pair<int, std::string>(error_code, filename));
}

long int server::get_client_body_size( ) const {
    return _client_body_size;
}

void server::set_client_body_size(long int size) {
	_client_body_size = size;
}

const std::list<route>& server::get_routes( ) const {
    return _routs;
}

void server::add_route(const route &route_)
{
	_routs.push_back(route_);
}

std::string server::request_processing( const std::string &request, std::string const & def_file ) {
    if (is_file(request))
        return request;
    else
        return request + def_file;
}

bool server::is_file( std::string request ) {
    int     ret;
    std::string::iterator it = request.begin();

    ret = static_cast<int>(request.rfind('/'));
    if (ret == -1)
        return false;
    else
    {
        it += ret;
        while (it != request.end())
        {
            if (*it++ == '.')
                return true;
        }
    }
    return false;
}

std::pair<std::string, std::string> server::split_request( const std::string &request ) {
    std::pair<std::string, std::string> ret;
    std::string     tmp;

    int n = (int)request.find('?');
    ret.second = request.substr(n + 1, request.length());
    try {
        ret.first = get_path_to_request(request.substr(0, n));
    }
    catch (std::exception &e)
    {
        throw std::exception();
    }
    return ret;
}

std::ostream &operator<<(std::ostream &o, const server &serv) {
	o << "\tserver:" << std::endl;
	o << "\t\thost: " << serv.get_host() << std::endl;
	o << "\t\tport: " << serv.get_port() << std::endl;
	o << "\t\tserver_names:";
	for (std::list<std::string>::const_iterator it = serv.get_server_names().begin();
		it != serv.get_server_names().end(); ++it) {
		o << " " << *it;
	}
	o << std::endl;
	o << "\t\terror_pages: " << std::endl;
	for (std::map<int, std::string>::const_iterator it = serv.get_error_pages().begin();
		 it != serv.get_error_pages().end(); ++it) {
		o << "\t\t\t" << it->first << ": " << it->second << std::endl;
	}
	o << "\t\tclient_max_body_size: " << serv.get_client_body_size() << std::endl;
	for (std::list<route>::const_iterator it = serv.get_routes().begin();
		it != serv.get_routes().end(); ++it) {
		o << *it;
	}
	return o;
}
