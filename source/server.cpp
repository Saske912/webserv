//
// Created by Pamula File on 5/8/21.
//

#include "../header.h"
#include "server.hpp"
#include "Number.hpp"
#include "header.hpp"

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
    _set = src._set;
    _host_socket = src._host_socket;
    read = src.read;
    addr = src.addr;
    _conf = src._conf;
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

bool server::is_file_with_extension( std::string request )
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

void server::concat( Header & head )
{
    std::list<std::string> lom(getListOfMethods());
//    head.setHostHeaderResponse(get_host(), get_port());
    if (std::find(lom.begin(), lom.end(), head.getMethod()) == lom.end())
        head.setFile(exception_processing(501, head));
    std::list<route>::iterator it = _routs.begin();
    while (it != _routs.end())
    {
        if (it->check_name(head.getRequest()))
        {
            head.setRout(&(*it));
            head.setRealPathToFile(it->swap_path(head.getRequest()));
            head.setContent_Location(set_location(*head.getRout(), head));
            return ;
        }
        it++;
    }
    head.setFile(exception_processing(404, head));
}

std::string server::get_error(int err, std::map<int, std::string> ers) {
    return ers[err];
}

int server::exception_processing( int except, Header &head )
{
    int     pid;
    char    **arg = (char **) malloc(sizeof(char *) * 4 + 1);
    int     fds[2];
    std::string concat;
    std::string to_head;
    int     ret;
    char    **env = NULL;

    if (!arg)
        error_exit("server::exception_processing malloc 1");
    to_head = get_error(except, _error_pages);
    head.setError(except);
	head.setPermission(true);
    if (to_head.length())
    {
        ret = chdir(head.getEnvValue("PWD=").c_str());
        if (ret == -1)
            error_exit("chdir in exception_processing");
        head.setResponse(const_cast<char *>(("HTTP/1.1 " + ttostr(except) + " "\
        + get_error(except, _default_error_pages)).c_str()));
        ret = open(to_head.c_str(), O_RDONLY);
        if (ret == -1)
            error_exit("open in exception_processing");
        free(arg);
        return ret;
    }
    else
    {
        to_head = get_error(except, _default_error_pages);
        ret = pipe(fds);
        if (ret == -1)
            error_exit("pipe in exception_processing");
        if ((pid = fork()) == 0)
        {
            concat = "s/SWAP/";
            arg[0] = strdup("content/sed.sh");
            if (!arg[0])
                exit(2);
            concat += to_head + "/";
            arg[1] = strdup(concat.c_str());
            if (!arg[1])
                exit(2);
            arg[2] = strdup("content/error_template.html");
            if (!arg[2])
                exit(2);
            arg[3] = NULL;
            close(fds[0]);
            dup2(fds[1], 1);
            try{env = head.env_to_char();}
            catch(std::exception &e){std::cout << e.what() << std::endl;}
			if (!env)
				exit(2);
            if (execve(arg[0], arg, env) == -1)
                perror("execve");
            free(arg);
            exit(3);
        }
        else if (pid == -1)
            error_exit("fork error in exception_processing");
        close(fds[1]);
        int     stat;
        if (pid > 0)
            waitpid(pid, &stat, 0);
        if ( WIFSIGNALED(stat))
        {
            std::cerr << "WTERMSIG: " << WTERMSIG(stat) << std::endl;
            std::cerr << "WCOREDUMP: " << WCOREDUMP(stat) << std::endl;
        }
        else if (WIFSTOPPED(stat))
            std::cerr << "WSTOPSIG: " << WSTOPSIG(stat) << std::endl;
//            error_exit("system error in fork");
        head.setResponse(const_cast<char *>((head.getHttp() + " " + ttostr(except)\
        + " " + to_head).c_str()));
        free(arg);
        return fds[0];
    }
}

void server::descriptorForSend( Header &head )
{
    int     ret;

    if ( !head.isClientNowInQueue())
    {
        if (head.getError())
        {
            head.setResponse(head.getHttp() + " " + ttostr(head.getError()) + " " + _default_error_pages[head.getError()]);
            return ;
        }
        else
            head.setResponse(head.getHttp() + " 200 OK");
        ret = chdir(head.getEnvValue("PWD=").c_str());
        if (ret == -1)
            error_exit("chdir in server::descriptorForSend");
        if (head.getMethod() != "DELETE")
            is_cgi( head );
        if (head.getFile())
            return ;
        else if (head.getMethod() == "GET" or head.getMethod() == "HEAD")
        {
            ret = open(head.getRealPathToFile().c_str(), O_RDONLY);
            if (ret == -1)
                error_exit("open in server::descriptorForSend");
            head.setFile(ret);
        }
        else if (head.getMethod() == "DELETE")
        {
            remove(head.getRealPathToFile().c_str());
            head.setFile(999);
        }
        else
            head.setContent_Length("0");
    }
}

int server::descriptorForReceive( Header & head)
{
    std::string real_path(head.getRealPathToFile());
    std::string part;
    int     fd;
    int     ret;

    if ( !head.isClientNowInQueue())
    {
        if (head.getError())
        {
            ret = open((real_path + ".rec").c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if (ret == -1)
                error_exit("open 1 in server::descriptorForReceive");
            return ret;
        }
        if ((fd = open(real_path.c_str(), O_RDONLY)) == -1)
            part = "201 Created";
        else
            part = "204 No Content";
        head.setResponse(head.getHttp() + " " + part);
        close(fd);
        ret = open(real_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if (ret == -1)
            error_exit("open 2 in server::descriptorForReceive");
        return ret;
    }
    else
        return 0;
}

void server::handle_cgi_response_headers(int fd, Header &head) {
    char *line;
    int status = 1;
    int skip = 0;

    while (status == 1) {
        status = get_next_line(fd, &line);
        if (status == -1)
            break;
        status = parse_cgi(head, line);
        if (!status) {
            skip += strlen(line) + 1;
        }
        if (status) {
            if (std::string("\r") == line)
                skip += 2;
            free(line);
            break;
        }
        free(line);
        status = 1;
    }
    lseek(fd, 0, skip);
}

#ifndef BONUS

void server::cgi_processing( Header &head, bool flag )
{
    int     fd1 = dup(1);
    int     fd0 = dup(0);
    std::string root;
    int     file;
    int     response_buffer;
    pid_t   pid;
    char    *arg[2];
    char    **env;
    int     stat = 0;

    head.setIsCgi(true);
    if (!flag)
        root = head.getRout()->get_cgi().second;
    else
        root = _cgi_path;
    root = get_path_to_cgi(root, head.getEnvValue("PATH="), head.getEnvValue("PWD="));
    if (root.empty())
    {
        head.setFile(exception_processing(500, head));
        return ;
    }
    file = open(head.getRealPathToFile().c_str(), O_RDONLY);
    if (file == -1)
        head.setFile(exception_processing(500, head));
    response_buffer = open((head.getRealPathToFile() + ".tmp").c_str(), O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (response_buffer == -1)
        head.setFile(exception_processing(500, head));
    else
        head.setFile(response_buffer);
    int ret = chdir(head.getRout()->get_root().c_str());
    if (ret == -1)
        error_exit("chdir in cgi_processing");
    if ((pid = fork()) == 0)
    {
        arg[0] = strdup(root.c_str());
        arg[1] = NULL;
        dup2(file, 0);
        dup2(response_buffer, 1);
        env = head.env_to_char();
        if (!env)
            throw std::exception();
        execve(arg[0], arg, env);
        exit(1);
    }
    else if (pid == -1)
    {
        head.setFile(exception_processing(500, head));
        return ;
    }
    head.setPid(pid);
    waitpid(pid, &stat, 0);
    close(file);
    if ( WIFSIGNALED(stat))
    {
        std::cerr << "WTERMSIG: " << WTERMSIG(stat) << std::endl;
        std::cerr << "WCOREDUMP: " << WCOREDUMP(stat) << std::endl;
    }
    else if (WIFSTOPPED(stat))
        std::cerr << "WSTOPSIG: " << WSTOPSIG(stat) << std::endl;
    else if ( WIFEXITED(stat) and WEXITSTATUS(stat))
    {
        std::cerr << WEXITSTATUS(stat)  << std::endl;
        error_exit("error in fork");
    }
    lseek(response_buffer, 0, 0);
    handle_cgi_response_headers(response_buffer, head);
    dup2(fd1, 1);
    dup2(fd0, 0);
    close(fd1);
    close(fd0);
    ret = chdir(head.getEnvValue("PWD=").c_str());
    if (ret == -1)
        error_exit("chdir(2) in cgi_processing");
}
#endif
#ifdef BONUS

void server::cgi_processing( Header &head, bool flag )
{
    int     fd1 = dup(1);
    int     fd0 = dup(0);
    std::string root;
    int     file;
    int     response_buffer;
    pid_t   pid;
    char    *arg[2];
    char    **env;
    int     stat = 0;

    head.setIsCgi(true);
    if (!flag)
        root = head.getRout()->get_cgi().second;
    else
        root = _cgi_path;
    root = get_path_to_cgi(root, head.getEnvValue("PATH="), head.getEnvValue("PWD="));
    if (root.empty())
    {
        head.setFile(exception_processing(500, head));
        return ;
    }
    file = open(head.getRealPathToFile().c_str(), O_RDONLY);
    if (file == -1)
        head.setFile(exception_processing(500, head));
    struct ::stat stats;
    int ret;
	if (head.getMethod() == "GET" && (ret = ::stat((head.getRealPathToFile() + ".cookie").c_str(), &stats)) != -1)
	{
		response_buffer = open((head.getRealPathToFile() + ".cookie").c_str(), O_RDONLY);
		if (response_buffer == -1)
			head.setFile(exception_processing(500, head));
		else
			head.setFile(response_buffer);
		return ;
	}
	else
	{
	   if (head.getMethod() == "GET")
	   {
			if ((response_buffer = open((head.getRealPathToFile() + ".cookie").c_str(), O_RDWR | O_CREAT | O_TRUNC, 0777)) < 0)
				error_exit("open error");
			head.setFile(response_buffer);
	   }
	   else
	   {
			response_buffer = open((head.getRealPathToFile() + ".tmp").c_str(), O_RDWR | O_CREAT | O_TRUNC, 0777);
			if (response_buffer == -1)
			{
				 head.setFile(exception_processing(500, head));
				 return ;
			}
			else
				head.setFile(response_buffer);
	   }
	}
	ret = chdir(head.getRout()->get_root().c_str());
	if (ret == -1)
		error_exit("chdir in cgi_processing");
    if ((pid = fork()) == 0)
    {
        arg[0] = strdup(root.c_str());
        arg[1] = NULL;
        dup2(file, 0);
        dup2(response_buffer, 1);
        env = head.env_to_char();
        if (!env)
            throw std::exception();
        execve(arg[0], arg, env);
        exit(1);
    }
    else if (pid == -1)
    {
        head.setFile(exception_processing(500, head));
        return ;
    }
    head.setPid(pid);
    waitpid(pid, &stat, 0);
    close(file);
    if ( WIFSIGNALED(stat))
    {
        std::cerr << "WTERMSIG: " << WTERMSIG(stat) << std::endl;
        std::cerr << "WCOREDUMP: " << WCOREDUMP(stat) << std::endl;
    }
    else if (WIFSTOPPED(stat))
        std::cerr << "WSTOPSIG: " << WSTOPSIG(stat) << std::endl;
    else if ( WIFEXITED(stat) and WEXITSTATUS(stat))
    {
        std::cerr << WEXITSTATUS(stat)  << std::endl;
        error_exit("error in fork");
    }
    lseek(response_buffer, 0, 0);
    handle_cgi_response_headers(response_buffer, head);
    dup2(fd1, 1);
    dup2(fd0, 0);
    close(fd1);
    close(fd0);
	ret = chdir(head.getEnvValue("PWD=").c_str());
	if (ret == -1)
		error_exit("chdir(2) in cgi_processing");
}
#endif

void server::is_cgi( Header &head )
{
    const   std::string& ext(head.getExtension());
    if (ext == head.getRout()->get_cgi().first)
        cgi_processing( head, false );
    else if (_allow.first == ext and _allow.second == head.getMethod())
        cgi_processing( head, true );
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
//    _list_of_methods.push_back("OPTIONS");
    _list_of_methods.push_back("GET");
    _list_of_methods.push_back("HEAD");
    _list_of_methods.push_back("POST");
    _list_of_methods.push_back("PUT");
    _list_of_methods.push_back("DELETE");
//    _list_of_methods.push_back("TRACE");
//    _list_of_methods.push_back("CONNECT");
}

std::string server::set_location(route & route, Header & head) {
    if ( route.get_default_page().empty() or is_file_with_extension( head.getRequest( )))
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

int server::autoindex( Header &head, route route )
{
    DIR     *dir;
    dirent  *dir_p;
    int     fd;

    head.setResponse("HTTP/1.1 200 OK\r\n");
    fd = open("autoindex.html", O_RDWR | O_CREAT | O_TRUNC, 0777 );
    if (!(dir = opendir(("./" + route.swap_path(head.getRequest())).c_str())))
        return exception_processing(404, head);
    std::string str = "<!DOCTYPE html>\n"
                      "<html lang=\"en\">\n"
                      "<head>\n"
                      "\t<meta charset=\"UTF-8\">\n"
                      "\t<title>directory listing of " +
                      head.getRequest() +
                      "</title>\n"
                      "</head>\n"
                      "<body>\n";
    write(fd, str.c_str(), str.length());
    str = "\t<h1>Directory listing of " + head.getRequest() + "</h1>\n";
    write(fd, str.c_str(), str.length());
    write(fd, "\t<hr>\n", 6);
    std::string temp;
    while ((dir_p = readdir(dir)))
    {
        if (((head.getRequest().empty() || head.getRequest() == "/") &&
            dir_p->d_name == std::string("..")) ||
            dir_p->d_name == std::string("."))
            continue;
        str = "\t<p>\n";
        temp = *(head.getRequest()).rbegin() == '/' ? head.getRequest() : head.getRequest() + '/';
        if (dir_p->d_type & DT_DIR)
            str += "\t\t<a href=\"" + temp + std::string(dir_p->d_name) + "/" + "\">" + std::string(dir_p->d_name) + "/</a>\n";
        else
            str += "\t\t<a href=\"" + temp + std::string(dir_p->d_name) + "\">" + std::string(dir_p->d_name) + "</a>\n";
        str += "\t</p>\n";
        write(fd, str.c_str(), str.length());
    }
    write(fd, "\t<hr>\n", 6);
    closedir(dir);
    str = "</body>\n"
          "</html>";
    write(fd, str.c_str(), str.length());
    lseek(fd, 0, 0);
    return fd;
}

bool server::is_file( std::string const & request ) {
    struct ::stat st;
    std::string part;
    ::stat(request.c_str(), &st);
    if (st.st_mode & S_IFREG)
        return true;
    return false;
}

const std::list<std::string> &server::getListOfMethods( ) const {
    return _list_of_methods;
}

void server::setListOfMethods( const std::list<std::string> &listOfMethods ) {
    _list_of_methods = listOfMethods;
}

std::list<Header> &server::getSet( ) {
    return _set;
}

void server::setSet( const std::list<Header> &set ) {
    _set = set;
}

int server::getHostSock( ) const {
    return _host_socket;
}

void server::setHostRaw( int hostRaw ) {
    _host_socket = hostRaw;
}

config *server::getConf( ) const {
    return _conf;
}

void server::setConf( config *conf ) {
    _conf = conf;
}

bool server::head_in_set(Header &head)
{
    std::list<Header>::iterator it(_set.begin());
    std::list<Header>::iterator ite(_set.end());

    while (it != ite)
    {
        if (&(*it) == &head)
            return true;
        it++;
    }
    return false;
}

bool server::operator==(Header const & head) const {
    return _port == head.getPort();
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
