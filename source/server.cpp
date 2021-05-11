//
// Created by Pamula File on 5/8/21.
//

#include "server.hpp"
#include <iostream>

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

int server::get_path_to_request( const std::string &request, Header & head ) {
    std::list<route>::iterator it = _routs.begin();
    while (it != _routs.end())
    {
        if (!(*it).check_name(dirs(request)))
        {
            return request_processing((*it).swap_path(request), (*it).get_default_page(), *it, head);
        }
        it++;
    }
    return exception_processing(404, head);
}

const std::map<int, std::string>& server::get_error_pages( ) const {
    return _error_pages;
}

void server::set_error_pages( const std::map<int, std::string> &err_pages )
{
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

int     server::request_processing( const std::string &request, \
std::string const & def_file, route const & route, Header & head ) {
	(void)route;
	if (is_file(request))
		return targeting(head, request);
	else
		return targeting(head, request + def_file);
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
            std::cout << *it  << std::endl;
            if (*it++ == '.')
                return true;
        }
    }
    return false;
}

int    server::responce( Header & head ) {
    std::pair<int, std::string> ret;
    std::string                 request;
    std::string                 tmp;

    request = head.getRequest();
    int n = (int)request.find('?');
    if (n > 0)
    {
        tmp = request.substr(0, n);
        ret.first = get_path_to_request(tmp, head);
        ret.second = request.substr(n + 1, request.length());
    }
    else
        ret.first = get_path_to_request(request, head);
    return ret.first;
}

std::string server::dirs( std::string request ) {
    return request;
}

char *server::get_error(int err) {
    std::string tmp = _error_pages[err];
    return strdup(const_cast<char *>(tmp.c_str( )));
}

int server::exception_processing( int except, Header &head ) {
    int     pid;
    int     stat;
    char    **arg;
    int     fds[2];

    pipe(fds);
    if ((pid = fork()) == 0)
    {
        arg = reinterpret_cast<char **>(ft_calloc(4, sizeof(char **)));
        arg[0] = strdup("/usr/bin/sed");
        arg[1] = strdup("s/SWAP/Not Found/");//        get_error(except);
        arg[2] = strdup("content/error_template.html");
        head.setMethod("Not Found");
        close(fds[0]);
        dup2(fds[1], 1);
        execve(arg[0], arg, head.getEnv());
        exit(1);
    }
    close(fds[1]);
    waitpid(pid, &stat, 0);
    if (stat == 1)
        std::cout << "error"  << std::endl;
    head.setHttp("HTTP/1.1 ");
    head.setRequest(ft_itoa(except));
//    head.setMethod(get_error(except));
    head.setMethod("Not Found");
    return fds[0];
}

int server::targeting( Header &head, std::string request ) {
    int     fd;


    if ( (fd = open(request.c_str(), O_RDONLY)) == -1)
    {
//        if (errno == EACCES) {
            fd = exception_processing(403, head);
//        }
    }
    else
    {
        head.setHttp("HTTP/1.1 ");
        head.setRequest("200");
        head.setMethod("OK");
    }
    return fd;
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
