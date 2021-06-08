//
// Created by Pamula File on 5/8/21.
//

#include "../header.h"
#include "server.hpp"

server::server() : _server_names(), _error_pages(), _routs(),
    _client_body_size(~0 ^ long(1L << (sizeof(long) * 8 - 1))),
    _allow(), _cgi_path() {
    set_default_pages();
    set_list_of_methods();
}

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
    this->_list_of_methods = src._list_of_methods;
    this->_allow = src._allow;
    this->_cgi_path = src._cgi_path;
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
    set_list_of_methods();
}

int server::get_path_to_request( const std::string &request, Header & head) {
    std::list<route>::iterator it = _routs.begin();
    while (it != _routs.end())
    {
        chdir(it->get_root().c_str());
        std::cout << "request: " << request  << std::endl;
        if (!(*it).check_name(request))
        {
            if (!check_methods(head.getMethod(), it->get_http_methods()) and !is_allow(request, head.getMethod(), *it))
            {
                head.setAllow(get_allow(it->get_http_methods()));
                return exception_processing(405, head);
            }
            else if (head.getMethod() == "GET" || head.getMethod() == "PUT" || head.getMethod() == "POST")
                return request_processing((*it).swap_path(request), (*it).get_default_page(), *it, head);
            else if (head.getMethod() == "HEAD")
                return -1;
//            else if (head.getMethod() == "PUT")
//                return request_processing((*it).swap_path(request), (*it).get_default_page(), *it, head);
//            else if (head.getMethod() == "POST") {
//                return request_processing(( *it ).swap_path( request ), ( *it ).get_default_page( ), *it, head );
//            }
        }
        it++;
    }
    std::cerr << "Path not found: " << request << std::endl;
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
std::string const & def_file, route const & route, Header & head) {
                                    std::cout << "reqest(requst_processing): " << request  << std::endl;
	if ( is_file_with_extension( request ) or head.getMethod() == "PUT")
    {
        return targeting(head, request, route);
    }
	else
    {
        if (def_file.empty() and route.get_autoindex())
        {
            return autoindex(route.get_root(), head, route.get_name());
        }
        else if (request == "/")
            return targeting(head, def_file, route);
        else if (*request.rbegin() != '/' and *def_file.begin() != '/')
            return targeting(head, request + '/' + def_file, route);
    }
    return targeting(head, request + def_file, route);
}

bool server::is_file_with_extension( std::string request )
{
//    int ret = static_cast<int>(request.rfind('/'));
//    if (ret == -1)
//        return false;
//    else
    {
        std::string::reverse_iterator it = request.rbegin();
        while(it != request.rend() and *it != '/')
        {
            if (*it == '.')
                return true;
            it++;
        }
        return false;
    }
}

int    server::responce( Header & head )
{
    std::pair<int, std::string> ret;
    std::string                 request;
    std::string                 tmp;
    std::string server_name = "SERVER_NAME=";
    std::string server_port = "SERVER_PORT=";

    request = head.getRequest();
    server_name += *_server_names.begin();
    head.addEnv(const_cast<char *>(server_name.c_str()));
    server_port += std::to_string(static_cast<int>(_port));
    head.addEnv(const_cast<char *>(server_port.c_str()));
    head.addEnv((char *)("PATH_INFO=" + head.getRequest()).c_str());
    if (head.getHost() == "400" || head.getHost().empty())
        return exception_processing(400, head);
    if (std::find(_list_of_methods.begin(), _list_of_methods.end(), head.getMethod()) == _list_of_methods.end())
        return exception_processing(501, head);
    head.setHost("Host: " + _host + ":" + std::to_string(static_cast<int>(_port)) + '\n');
    int n = (int)request.find('?');
    if (n > 0)
    {
        tmp = request.substr(0, n);
        ret.second = request.substr(n + 1, request.length());
        ret.first = get_path_to_request(tmp, head);
    }
    else
        ret.first = get_path_to_request(request, head);
    return ret.first;
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
    char    *arg[4];
    int     fds[2];
    std::string concat;
    std::string to_head;

    try
    {
        to_head = get_error(except, _error_pages);
        head.setResponse(const_cast<char *>(("HTTP/1.1 " + std::string(std::to_string(except)) + " "\
        + get_error(except, _default_error_pages) + "\r\n").c_str()));
        return open(to_head.c_str(), O_RDONLY);
    }
    catch (std::exception &)
    {
        to_head = get_error(except, _default_error_pages);
        pipe(fds);
        if ((pid = fork()) == 0)
        {
            concat = "s/SWAP/";
            arg[0] = strdup("content/sed.sh");
            concat += to_head + "/";
            arg[1] = strdup(concat.c_str());
            arg[2] = strdup("content/error_template.html");
            arg[3] = NULL;
            close(fds[0]);
            dup2(fds[1], 1);
            execve(arg[0], arg, head.getEnv());
            exit(1);
        }
        close(fds[1]);
        if (pid > 0)
            waitpid(pid, &stat, 0);
        if (stat == 1)
            error_exit("system error in execve");
        head.setResponse(const_cast<char *>(("HTTP/1.1 " + std::string(std::to_string(except))\
        + " " + to_head + "\r\n").c_str()));
        return fds[0];
    }
}

int server::targeting( Header &head, std::string request, route const & route ) {
    int     fd;
    int     pid;
    char    *arg[3];
    int     fdset[2];
    int     tmp;

    head.setContent_Location("Content-Location: " + set_location(const_cast<class route &>(route), head) + "\r\n");
    head.addEnv((char *)("SCRIPT_NAME=" + std::string(request, request.rfind('/') + 1, request.length() - request.rfind('/'))).c_str());
    if ((head.getMethod() == "PUT" or head.getMethod() == "POST") and head.getFd() == 1)
    {
        struct ::stat st;
        std::string part;
        ::stat(request.c_str(), &st);
        if (st.st_mode & S_IFDIR)
        {
            std::cout << "is_dir"  << std::endl;
            return exception_processing(404, head);
        }
        else if (errno == ENOENT)
            part = "201 Created\r\n";
        else
            part = "204 No Content\r\n";
        if ( (fd = open(request.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1)
        {
            if (errno == EACCES) {
                return exception_processing(403, head);
            }
            else
            {
                std::cout << "bad request: " << request  << std::endl;
                return exception_processing(404, head);
            }
        }
        else
            head.setResponse("HTTP/1.1 " + part);
    }
    else if ((is_cgi(request, route)) and (_allow.first.empty() or head.getMethod() == _allow.second))
    {
//        int     fd1 = dup(1);
//        int     fd0 = dup(0);

        head.setIsCgi(true);
        std::cout << "CGI start req:" << request << std::endl;
        if ((tmp = open(request.c_str(), O_RDONLY)) < 0) {
            error_exit( "open_error" );
        }
//        pipe(fdset);
        int fd;
        if ((fd = open("tmp", O_RDWR | O_CREAT | O_TRUNC, 0777)) < 0)
            error_exit("open error");
        if ((pid = fork()) == 0)
        {
            close(fdset[0]);
            arg[0] = strdup("../cgi.sh");
            arg[1] =  strdup(const_cast<char *>(route.get_cgi().second.c_str()));
            arg[2] = NULL;
//            arg[0] = strdup("cgi_tester");
//            arg[2] = strdup(const_cast<char *>(route.get_cgi().second.c_str()));
//            arg[1] = strdup(const_cast<char *>(request.c_str()));
            dup2(tmp, 0);
//            std::cout << "root: " << route.get_root()  << std::endl;
//            chdir(route.get_root().c_str());
//            char  buf[150];
//            getcwd(buf, 150);
//            std::cout << "getcwd(): " << buf  << std::endl;
//            dup2(fdset[1], 1);
            dup2(fd, 1);
            execve(arg[0], arg, head.getEnv());
            exit(1);
        }
        else if (pid == -1)
            error_exit("fork_error");
        else
            close(fdset[1]);
        int stat = 1;
        waitpid(pid, &stat, 0);
//        chdir(getenv("OLDPWD"));
        std::cout << "CGI ret fd"  << std::endl;
        lseek(fd, 0, 0);
//        close(fd);
//        dup2(fd1, 1);
//        if ((fd = open("tmp", O_RDONLY)) < 0)
//            error_exit("open error");
        return fd;
//        return fdset[0];
//        dup2(fd1, 1);
//        dup2(fd0, 0);
    }
    else
    {
        struct ::stat st;
        ::stat(request.c_str(), &st);
//        chdir(route.get_root().c_str());
        std::cout << "debug here"  << std::endl;
        if (st.st_mode & S_IFDIR)
        {
            std::cout << "is_dir(GET file)"  << std::endl;
            return exception_processing(404, head);
        }
        if ( (fd = open(request.c_str(), O_RDONLY)) == -1)
        {
            if (errno == EACCES)
                return exception_processing(403, head);
            else
            {
                std::cout << "bad request:(GET) " << request  << std::endl;
                return exception_processing(404, head);
            }
        }
        else
            head.setResponse("HTTP/1.1 200 OK\r\n");
    }
    return fd;
}

bool server::is_cgi( const std::string& request, route  const & route ) const {
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

void server::set_list_of_methods( ) {
    _list_of_methods.push_back("OPTIONS");
    _list_of_methods.push_back("GET");
    _list_of_methods.push_back("HEAD");
    _list_of_methods.push_back("POST");
    _list_of_methods.push_back("PUT");
    _list_of_methods.push_back("DELETE");
    _list_of_methods.push_back("TRACE");
    _list_of_methods.push_back("CONNECT");
}

std::string server::set_location(route & route, Header & head) {
    if ( route.get_default_page().empty() or is_file( head.getRequest( )))
        return head.getRequest();
    else
    {
        if (*(head.getRequest().rbegin()) != '/' and *(route.get_default_page().begin()) != '/')
            return head.getRequest() + '/' + route.get_default_page();
        else
            return head.getRequest() + route.get_default_page();
    }
}

std::pair<std::string, std::string> server::getAllow( ) const {
    return _allow;
}

void server::setAllow( const std::pair<std::string, std::string>& allow ) {
    _allow = allow;
}

std::string server::getCgiPath() const
{
	return _cgi_path;
}

void server::setCgiPath(const std::string &cgi_path)
{
	_cgi_path = cgi_path;
}

bool server::is_allow( const std::string & request, std::string const & method, route r) const {
    if (!_allow.first.empty() and !_allow.second.empty() and method == _allow.second)
    {
        std::string tmp = r.get_default_page();
        std::cerr << "allowed "  << std::endl;
        if ( is_file_with_extension( request ))
        {
            if (std::string(request, request.rfind('.') + 1, request.length() - request.rfind('.')) == _allow.first)
                return true;
        }
        else if (!r.get_default_page().empty())
        {
            if (std::string(tmp, tmp.rfind('.') + 1, tmp.length() - tmp.rfind('.')) == _allow.first)
                return true;
        }
    }
    return false;
}

int server::autoindex( std::string const & root, Header & head, std::string name )
{
    DIR     *dir;
    dirent  *dir_p;
    int     fd;
    std::string tmp;

    fd = open("content/autoindex.html", O_RDWR | O_CREAT | O_TRUNC, 0777 );
    if (!(dir = opendir(root.c_str())))
        exception_processing(404, head);
    std::string str = "<!DOCTYPE html>\n"
                      "<html lang=\"en\">\n"
                      "<head>\n"
                      "    <meta charset=\"UTF-8\">\n"
                      "    <title>Title</title>\n"
                      "</head>\n"
                      "<body>";
    write(fd, str.c_str(), str.length());
    while ((dir_p = readdir(dir)))
    {
        tmp = "<a href=\"" + std::string(name + dir_p->d_name) + "\">" + std::string(dir_p->d_name) + "</a>\n";
        write(fd, tmp.c_str(), tmp.length());
    }
    closedir(dir);
    str = "</body>\n"
          "</html>";
    write(fd, str.c_str(), str.length());
    lseek(fd, 0, 0);
    return fd;
}

bool server::is_file( std::string request ) {
    struct ::stat st;
    std::string part;
    ::stat(request.c_str(), &st);
    if (st.st_mode & S_IFDIR)
        return false;
    return true;
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
	o << "\t\tallow: " << serv.getAllow().first << " | " << serv.getAllow().second << std::endl;
	o << "\t\tcgi_path: " << serv.getCgiPath() << std::endl;
	return o;
}
