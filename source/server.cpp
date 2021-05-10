//
// Created by Pamula File on 5/8/21.
//

#include "server.hpp"
#include <iostream>

server::server( void ) { }

server::~server( void ) { }

server::server( server const &src ) {
    *this = src;
}

server &server::operator=( server const &src ) {
    this->_host = src._host;
    this->_port = src._port;
    this->_routs = src._routs;
    this->_client_body_size = src._client_body_size;
//    this->_error_pages = src._error_pages;
//    this->_server_names = src._server_names;
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

std::list<std::string> server::get_server_names( ) const {
    return _server_names;
}

void server::set_server_name( const std::list<std::string> &server_name ) {
    _server_names = server_name;
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
    throw std::exception();
}

std::map<int, std::string> server::get_error_pages( ) const {
    return _error_pages;
}

void server::set_error_pages( const std::map<int, std::string> &err_pages ) {
    _error_pages = err_pages;
}

long int server::get_client_body_size( ) const {
    return _client_body_size;
}

std::list<route> server::get_routes( ) const {
    return _routs;
}

int     server::request_processing( const std::string &request, \
std::string const & def_file, route const & route, Header & head ) {
    int     fd;
//    int     pid;
//    int     stat;
//    char    **arg;
//    int     fds[2];

    if (is_file(request))
    {
        if ( (fd = open(request.c_str(), O_RDONLY)) == -1)
        {
//            pipe(fds);
//            if ((pid = fork()) == 0)
//            {
//                *arg = reinterpret_cast<char *>(ft_calloc(4, 1));
//                arg[0] = strdup("content/sed.sh");
//                arg[1] = strdup("content/error_template.html");
//                arg[2] = strdup("{}");
//                if (errno == EACCES)
//                {
//                    close(fds[0]);
//                    dup2(fds[1], 1);
//                    arg[3] = get_error(403);
//                    execve("content/sed.sh", arg, head.getEnv());
//                }
////                else if (errno = smth) ...
//            }
//            close(fds[1]);
//            waitpid(pid, &stat, 0);
//            return fds[0];
        }
//        head.
        return fd;
    }
    else
    {
        return open((request + def_file).c_str(), O_RDONLY);
    }
}

bool server::is_file( std::string request ) {
    int     ret;
    std::string::iterator it = request.begin();
    std::cout << "check1: ";
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
    tmp = request.substr(0, n);
    ret.first = get_path_to_request(tmp, head);
//        std::cout << "first: " << ret.first << std::endl;
    try
    {
        ret.second = request.substr(n + 1, request.length());
        std::cout << "second: " << ret.second  << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "no request"  << std::endl;
    }
    return ret.first;
}

std::string server::dirs( std::string ) {
    return std::string( );
}

char *server::get_error(int err) {
    std::string tmp = _error_pages[err];
    return strdup(const_cast<char *>(tmp.c_str( )));
}
