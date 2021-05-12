//
// Created by Pamula File on 5/8/21.
//

#include "../header.h"
#include "server.hpp"


server::server() : _server_names(), _error_pages(), _routs() {set_default_pages();}

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
    this->_default_error_pages = src._default_error_pages;
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
    set_default_pages();
}

int server::get_path_to_request( const std::string &request, Header & head ) {
    std::list<route>::iterator it = _routs.begin();
    while (it != _routs.end())
    {
        if (!(*it).check_name(dirs(request)))
        {
            if (!check_methods(head.getMethod(), it->get_http_methods()))
            {
                head.setAllow(get_allow(it->get_http_methods()));
                return exception_processing(405, head);
            }
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
	if (is_file(request))
		return targeting(head, request, route);
	else
    {
	    if (*request.rbegin() != '/' and *def_file.begin() != '/')
            return targeting(head, request + '/' + def_file, route);
        return targeting(head, request + def_file, route);
    }
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

std::string server::get_error(int err, std::map<int, std::string> ers) {
    std::string tmp = ers[err];
    if (tmp.empty())
        throw std::exception();
    return tmp;
}

int server::exception_processing( int except, Header &head ) {
    int     pid;
    int     stat;
    char    **arg;
    int     fds[2];
    std::string concat;
    std::string to_head;

    try
    {
        to_head = get_error(except, _error_pages);
        head.setHttp("HTTP/1.1 ");
        head.setRequest(ft_itoa(except));
        head.setMethod(get_error(except, _default_error_pages));
        return open(to_head.c_str(), O_RDONLY);
    }
    catch (std::exception &)
    {
        pipe(fds);
        if ((pid = fork()) == 0)
        {
            concat = "s/SWAP/";
            arg = reinterpret_cast<char **>(ft_calloc(4, sizeof(char **)));
            arg[0] = strdup("content/sed.sh");
            to_head = get_error(except, _default_error_pages);
            concat += to_head + "/";
            arg[1] = strdup(concat.c_str());
            arg[2] = strdup("content/error_template.html");
            close(fds[0]);
            dup2(fds[1], 1);
            execve(arg[0], arg, head.getEnv());
            exit(1);
        }
        close(fds[1]);
        waitpid(pid, &stat, 0);
        if (stat == 1)
            error_exit("system error in execve");
        head.setHttp("HTTP/1.1 ");
        head.setRequest(ft_itoa(except));
        head.setMethod(to_head);
        return fds[0];
    }
}

int server::targeting( Header &head, std::string request, route const & route ) {
    int     fd;
    int     pid;
    int     stat;
    char    **arg;

    head.setContent_Location(request);
    if (is_сgi(request, route))
    {
        if ((fd = open("tmp", O_RDWR | O_CREAT | O_TRUNC, 655)) < 0)
            error_exit("open error");
        if ((pid = fork()) == 0)
        {
            arg = (char **)ft_calloc(2, sizeof(char **));
            arg[0] = const_cast<char *>(request.c_str());
            dup2(fd, 1);
            execve(arg[0], arg, head.getEnv());
        }
        else if (pid == -1)
            error_exit("fork_error");
        waitpid(pid, &stat, 0);
        lseek(fd, 0, 0);
        head.setHttp("HTTP/1.1 ");
        head.setRequest("200");
        head.setMethod("OK");
        return fd;
    }
    else
    {
        if ( (fd = open(request.c_str(), O_RDONLY)) == -1)
        {
            if (errno == EACCES) {
                fd = exception_processing(403, head);
            }
            else
                fd = exception_processing(404, head);
        }
        else
        {
            head.setHttp("HTTP/1.1 ");
            head.setRequest("200");
            head.setMethod("OK");
        }
    }
    return fd;
}

bool server::is_сgi( const std::string& request, route  const & route ) const {
    return request.substr(request.rfind('.') + 1, request.length()) == route.get_cgi().first;
}

void server::set_default_pages( ) {
    _default_error_pages[100] = "Continue";
    _default_error_pages[101] = "Switching Protocols";

    _default_error_pages[200] = "OK";
    _default_error_pages[201] = "Created";
    _default_error_pages[202] = "Accepted";
    _default_error_pages[203] = "Non-Authoritative Information";
    _default_error_pages[204] = "No Content";
    _default_error_pages[205] = "Reset Content";
    _default_error_pages[206] = "Partial Content";

    _default_error_pages[300] = "Multiple Choices";
    _default_error_pages[301] = "Moved Permanently";
    _default_error_pages[302] = "Found";
    _default_error_pages[303] = "See Other";
    _default_error_pages[304] = "Not Modified";
    _default_error_pages[305] = "Use Proxy";
    _default_error_pages[307] = "Temporary Redirect";

    _default_error_pages[400] = "Bad Request";
    _default_error_pages[401] = "Unauthorized";
    _default_error_pages[401] = "Payment Required";
    _default_error_pages[403] = "Forbidden";
    _default_error_pages[404] = "Not Found";
    _default_error_pages[405] = "Method Not Allowed";
    _default_error_pages[406] = "Not Acceptable";
    _default_error_pages[407] = "Proxy Authentication Required";
    _default_error_pages[408] = "Request Time-out";
    _default_error_pages[409] = "Conflict";
    _default_error_pages[410] = "Gone";
    _default_error_pages[411] = "Length Required";
    _default_error_pages[412] = "Precondition Failed";
    _default_error_pages[413] = "Request Entity Too Large";
    _default_error_pages[414] = "Request-URL Too Long";
    _default_error_pages[415] = "Unsupported Media Type";
    _default_error_pages[416] = "Requested range not satisfiable";
    _default_error_pages[417] = "Expectation Failed";

    _default_error_pages[500] = "Internal Server Error";
    _default_error_pages[501] = "Not Implemented";
    _default_error_pages[502] = "Bad Gateway";
    _default_error_pages[503] = "Service Unavailable";
    _default_error_pages[504] = "Gateway Timeout";
    _default_error_pages[505] = "HTTP Version not supported";
}

std::map<int, std::string> server::get_def_error_pages( ) const {
    return _default_error_pages;
}

bool server::check_methods( std::string str, std::list<std::string> arr ) const {
    std::list<std::string>::iterator it = arr.begin();
    while (it != arr.end())
    {
        if (str == *it++)
            return true;
    }
    return false;
}

std::string server::get_allow( std::list<std::string> arr ) {
    std::string ret = "Allow: ";
    std::list<std::string>::iterator it = arr.begin();
    while (it != arr.end())
    {
        ret += *it;
        if (*it != *arr.rbegin())
            ret += ", ";
        it++;
    }
    ret += '\n';
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
