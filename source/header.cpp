#include "header.hpp"
#include "header.h"
#include "Number.hpp"
#include "config.hpp"

std::string const Header::Last_Modified = "Last_Modified: " + get_current_date();
std::list<std::string>   Header::current_files_in_work;

void Header::eraseStruct()
{
	Accept_Charsets.erase();
	Accept_Language.erase();
	Allow.erase();
	Authorization.erase();
	Content_Language.erase();
	Content_Length.erase();
	Content_Location.erase();
	Content_Type.erase();
	Http.erase();
	Host.erase();
	Method.erase();
	Referer.erase();
	Request.erase();
	Retry_after.erase();
	Server.erase();
	Transfer_Encoding.erase();
	User_Agent.erase();
	WWW_Authenticate.erase();
	Port = 0;
//	env.clear();
    if (file)
	    close(file);
	if (receive_file)
	    close(receive_file);
    file = 0;
    receive_file = 0;
    error = 0;
    body_end = false;
    empty_line = false;
    client_now_in_queue = false;
    permission = false;
//	Fdr = 0;
	is_cgi = false;
	Pid = 0;
	BodySize = 0;
	buffer.clear();
	reminder.clear();
}

void Header::setAccept_Charsets(std::string const &str)
{
	Accept_Charsets = str;
}

void Header::setAccept_Language(std::string const &str)
{
	Accept_Language = "Accept-Language: " + str;
}

void Header::setAllow(std::string const &str)
{
	Allow = str;
}

void Header::setAuthorization(std::string const &str)
{
    std::string tmp(str);
    tmp.erase(0, strlen(AUTH));
	Authorization = str;
}

void Header::setContent_Language(std::string const &str)
{
	Content_Language = "Content-Language: " + str.substr(strlen(ACEPT_LANG)) + END;
}

void Header::setContent_Length(std::string const &str)
{
	Content_Length = "Content-Length: " + str + END;
}

void Header::setContent_Location(std::string const &str)
{
	Content_Location = "Content-Location: " + str + END;
}

void Header::setContent_Type(std::string const &str)
{
	Content_Type = "Content-Type: " + str + END;
}

void Header::setDate(std::string const &str)
{
	Date = "Date: " + str;
}

void Header::setHttp(std::string const &str)
{
	Http = "HTTP/" + str;
}

void Header::setHost(std::string const &str)
{
	Host = str;
}

void Header::setMethod(std::string const &str)
{
	Method = str;
}

void Header::setReferer(std::string const &str)
{
	Referer = str;
}

void Header::setRequest(std::string const &str)
{
	Request = str;
}

void Header::setResponse(std::string const &str)
{
	Response = str + END;
}

void Header::setRetry_after(std::string const &str)
{
	Retry_after = str;
}

void Header::setServer(std::string const &str)
{
	Server = str;
}

void Header::setTransfer_Encoding(std::string const &str)
{
    std::string tmp(str);
    tmp.erase(0, strlen(TRANS_ENC));
	Transfer_Encoding = tmp;
}

void Header::setUser_Agent(std::string const &str)
{
	User_Agent = str;
}

void Header::setWWW_Authenticate(std::string const &str)
{
	WWW_Authenticate = str;
}

void Header::setIsCgi(bool status) {
    is_cgi = status;
}

void Header::setEnv( const std::list<std::string> &env)
{
	Header::env = env;
}

void Header::setFile( int const &fd)
{
    struct stat st;
    std::stringstream stream;

    if (file)
        return;
    file = fd;
    fstat(getFile(), &st);
    stream << st.st_size;
    setContent_Length(stream.str());
}

//void Header::setFdr(int const &fd)
//{
//	Fdr = fd;
//}

void Header::setPort(unsigned int  const &port)
{
	Port = port;
}

void Header::setPid(pid_t  const &pid)
{
	Pid = pid;
}

void Header::setBodySize(int bodySize)
{
	BodySize = bodySize;
}

int	Header::getBodySize() const
{
	return BodySize;
}

pid_t Header::getPid()
{
	return Pid;
}

const std::list<std::string> & Header::getEnv()
{
    return env;
}

int Header::getFile()
{
	return file;
}
//
//int &Header::getFdr()
//{
//	return Fdr;
//}

unsigned int Header::getPort() const
{
	return Port;
}

std::string &Header::getAccept_Charsets()
{
	return Accept_Charsets;
}

std::string &Header::getAccept_Language()
{
	return Accept_Language;
}

std::string &Header::getAllow()
{
	return Allow;
}

std::string &Header::getAuthorization()
{
	return Authorization;
}

std::string &Header::getContent_Language()
{
	return Content_Language;
}

std::string &Header::getContent_Length()
{
	return Content_Length;
}

std::string &Header::getContent_Location()
{
	return Content_Location;
}

std::string &Header::getContent_Type()
{
	return Content_Type;
}

std::string &Header::getDate()
{
	return Date;
}

std::string &Header::getHttp()
{
	return Http;
}

std::string &Header::getHost()
{
	return Host;
}

std::string const &Header::getLast_Modified()
{
	return Last_Modified;
}

std::string &Header::getMethod()
{
	return Method;
}

std::string &Header::getReferer()
{
	return Referer;
}

std::string Header::getRequest()
{
	return Request;
}

std::string &Header::getResponse()
{
	return Response;
}

std::string &Header::getRetry_after()
{
	return Retry_after;
}

std::string &Header::getServer()
{
	return Server;
}

std::string &Header::getTransfer_Encoding()
{
	return Transfer_Encoding;
}

std::string &Header::getUser_Agent()
{
	return User_Agent;
}

std::string &Header::getWWW_Authenticate()
{
	return WWW_Authenticate;
}

bool &Header::getIsCgi() {
    return is_cgi;
}

std::string Header::getEnvValue(char const *str)
{
	int len = strlen(str);


	std::list<std::string>::iterator it = env.begin();
	while (it != env.end())
	{
		if (it->find(str) == 0)
			return std::string(*it, len, (*it).length() - len);
		++it;
	}
	return std::string();
}

void Header::addEnv( const std::string& str)
{
    std::string temp(str.substr(0, str.find('=')));
	std::list<std::string>::iterator it = env.begin();
    std::list<std::string>::iterator ite = env.end();
	while (it != ite)
	{
		if (it->find(temp) != std::string::npos)
		{
			env.erase(it);
			break ;
		}
		++it;
	}
	env.push_back( str);
}

void Header::put_string( std::string str ) {
    std::cout << str  << std::endl;
}

void Header::showEnv()
{
    std::list<std::string>::iterator it_env(env.begin());
    std::list<std::string>::iterator ite_env(env.end());

    while (it_env != ite_env)
        put_string(*it_env);
}

Header::Header()
{
}

void Header::setter( const std::string &line, server &serv )
{
    if (line.empty() && !getMethod().empty())
    {
        empty_line = true;
        setServ(&serv);
        cgi_env();
        if (getMethod() == "GET" || getMethod() == "HEAD")
            body_end = true;
        serv.concat( *this );
        if (getMethod() == "GET" || getMethod() == "HEAD" || getError())
        {
            serv.descriptorForSend( *this );
        }
        return ;
    }
    for (std::map<std::string, Func>::iterator it(array.begin()); it != array.end();)
    {
        if (line.find(it->first) != std::string::npos)
            return (this->*array[it->first])(line);
        ++it;
    }
}

void Header::http( std::string const & str )
{
    size_t  finder;
    std::string string(str);
    int     fd;

    fd = open("server.log", O_CREAT | O_WRONLY | O_APPEND, 0777);
    if (fd == -1)
        serv->exception_processing(500, *this);
    write(fd, str.c_str(), str.length());
    write(fd, "\n", 1);
    close(fd);
    setDate(get_current_date());
    if (string[0] == '\n')
        string.erase(0, 1);
    finder = string.find( ' ' );
    setMethod( string.substr( 0, finder));
    string.erase( 0, finder + 1);
    if ((finder = string.find( '?')) != std::string::npos)
    {
        setRequest( string.substr( 0, finder));
        string.erase( 0, finder + 1);
        finder = string.find( ' ');
        setQuery( string.substr( 0, finder));
        string.erase( 0, finder + 1);
    }
    else
    {
        finder = string.find( ' ' );
        setRequest( string.substr( 0, finder));
        string.erase( 0, finder + 1);
    }
    string.erase( 0, strlen( HTTP));
    setHttp(string);
}

void Header::host( const std::string &string )
{
    size_t  finder = string.find(' ') + 1;
    setHost(string.substr(finder, string.rfind(':') - finder));
    setPort(atoi(std::string(string, string.rfind(':') + 1).c_str()));
}

void Header::referer( const std::string &string )
{
    setReferer(std::string(string, string.find(' ') + 1));
}

void Header::accept( const std::string &string )
{
    setContent_Type(std::string(string, string.find(' ') + 1));
}

void Header::cgi_env( )
{
    std::string str;
    size_t      pos;

    addEnv("REQUEST_URI=" + getRequest());
    addEnv("REQUEST_METHOD=" + getMethod());
    addEnv("SERVER_PROTOCOL=" + getHttp());
    addEnv("PATH_INFO=" + getRequest());
    addEnv("PATH_TRANSLATED=" + getRequest());
    str = getRequest();
    if ((pos = str.find('?')) != std::string::npos)
        addEnv("QUERY_STRING=" + std::string(str, pos + 1));
    if (getContent_Type().find(',') != std::string::npos)
        addEnv("CONTENT_TYPE=" + std::string(getContent_Type(), 0, getContent_Type().find(',')));
    if (getAuthorization().find(' ') != std::string::npos)
        addEnv("AUTH_TYPE=" + std::string(getAuthorization(), 0, getAuthorization().find(' ')));
    addEnv("GATEWAY_INTERFACE=CGI/0.9");
    addEnv("REMOTE_ADDR=" + ip_addr);
    addEnv("SERVER_SOFTWARE=webserv/1.0 (Unix)");
    addEnv("PATH_INFO=" + getRequest());
    addEnv("HTTP_X_SECRET_HEADER_FOR_TEST=1");
    addEnv("SERVER_NAME=" + getServ()->get_server_names().front());
    addEnv("SERVER_PORT=" + ttostr(static_cast<int>(getServ()->get_port())));
//    if (std::string(getEnvValue("AUTH_TYPE=")) == "BASIC" || std::string(getEnvValue("AUTH_TYPE=")) == "DIGEST")
//    {
//        tmp = strchr(tmp, ' ') + 1;
//        addEnv((char *)("REMOTE_USER=" + std::string(tmp, 0, strchr(tmp, ':') - tmp)).c_str());
//        tmp = strchr(tmp, ':') + 1;
//        head.addEnv((char *)("REMOTE_IDENT=" + std::string(tmp)).c_str());
//    }
//    head.addEnv((char *)("SCRIPT_NAME=" + std::string(request, request.rfind('/') \
//    + 1, request.length() - request.rfind('/'))).c_str());
}

Header::Header( server &serv, const std::list<std::string> &env ) : rout(), serv(), Port(), ad(), adlen()
{
    int opt;
    int bufer = BUFSIZE;
    array.insert(std::pair<std::string, Func>(HTTP, &Header::http));
    array.insert(std::pair<std::string, Func>(ACEPT_LANG, &Header::setContent_Language));
    array.insert(std::pair<std::string, Func>(HOST, &Header::host));
    array.insert(std::pair<std::string, Func>(REFERER, &Header::referer));
    array.insert(std::pair<std::string, Func>(ACCEPT, &Header::accept));
    array.insert(std::pair<std::string, Func>(TRANS_ENC, &Header::setTransfer_Encoding));
    array.insert(std::pair<std::string, Func>(AUTH, &Header::setAuthorization));
    client_now_in_queue = false;
    Header::env = env;
    file = 0;
    receive_file = 0;
    error = 0;
    is_cgi = false;
    Pid = 0;
    BodySize = 0;
    empty_line = false;
    body_end = false;
    permission = false;
    if (( client = ::accept( serv.getHostSock( ), reinterpret_cast<sockaddr *>(&ad), &adlen)) == -1)
    {
        perror("accept");
    }
    fcntl(client, F_SETFL, O_NONBLOCK);
    opt = 1;
//    setsockopt( client, SOL_SOCKET, SO_NOSIGPIPE, reinterpret_cast<const void *>(serv.opt), sizeof(serv.opt));
    setsockopt(client, SOL_SOCKET, SO_SNDBUF, &bufer, sizeof(bufer));
    setsockopt(client, SOL_SOCKET, SO_RCVBUF, &bufer, sizeof(bufer));
    setsockopt(client, SOL_SOCKET, SO_NOSIGPIPE, &bufer, sizeof(bufer));
    ip_addr = ttostr(ad.sin_addr.s_addr & 255) + '.' +
            ttostr(ad.sin_addr.s_addr >> 8 & 255) + '.' +
            ttostr(ad.sin_addr.s_addr >> 16 & 255) + '.' +
            ttostr(ad.sin_addr.s_addr >> 24);
}

int Header::getClient( ) const {
    return client;
}

void Header::setServ( server *serv ) {
    Header::serv = serv;
}

server *Header::getServ( ) const {
    return serv;
}

const std::string &Header::getHostHeaderResponse( ) const {
    return host_header_response;
}

void Header::setHostHeaderResponse( const std::string &host, unsigned int port) {
    host_header_response = "Host: " + host + ':' + ttostr(port) + END;
}

int Header::getError( ) const {
    return error;
}

void Header::setError( int error ) {
    if (!Header::error)
        Header::error = error;
}

const std::string &Header::getQuery( ) const {
    return query;
}

void Header::setQuery( const std::string &query ) {
    Header::query = query;
}

const std::string &Header::getRealPathToFile( ) const {
    return real_path_to_file;
}

void Header::setRealPathToFile( const std::string &realPathToFile )
{
    struct stat st;
    std::string temp;
    size_t      finder;
    if (::stat(realPathToFile.c_str(), &st) == -1)
    {
        if (Method == "PUT" or Method == "POST")
        {
            temp = trim(realPathToFile, "/");
            finder = temp.rfind('/');
            if (finder == std::string::npos)
                setFile(serv->exception_processing(404, *this));
            else
            {
                temp = temp.substr(0, finder);
                if (::stat(temp.c_str(), &st) == -1)
                    setFile(serv->exception_processing(404, *this));
                else
                    real_path_to_file = realPathToFile;
            }
        }
        else
        {
            if (errno == EACCES)
                setFile(serv->exception_processing(403, *this));
            else
                setFile(serv->exception_processing(404, *this));
        }
    }
    else if (!(st.st_mode & S_IFREG))
    {
        if (st.st_mode & S_IFDIR)
        {
            if (rout->get_default_page().empty())
            {
                if (rout->get_autoindex())
                {
                    setError(1);
                    setFile(serv->autoindex(*this, *rout));
                }
            }
            else
            {
                real_path_to_file = rtrim(realPathToFile, "/") + "/" + ltrim(rout->get_default_page(), "/");
                if (::stat(real_path_to_file.c_str(), &st) == -1)
                {
                    if (errno == EACCES)
                        setFile(serv->exception_processing(403, *this));
                    else
                        setFile(serv->exception_processing(404, *this));
                }
            }
        }
    }
    else
        real_path_to_file = realPathToFile;
    if (real_path_to_file[0] != '/')
    {
        real_path_to_file = rtrim(getEnvValue("PWD="), "/") + '/' + ltrim(real_path_to_file, "/");
    }
    setExtension(real_path_to_file);
    check_rout();
    if ( file_available(real_path_to_file))
    {
        Header::current_files_in_work.push_back(real_path_to_file);
        permission = true;
    }
    else
    {
        serv->getConf()->moveToWait(*this, serv->getSet());
    }
}

const std::string &Header::getExtension( ) const {
    return extension;
}

void Header::setExtension( const std::string &filename )
{
    size_t      finder;
    std::string ext;

    finder = real_path_to_file.find('.') + 1;
    if (finder != std::string::npos)
        extension = filename.substr(finder);
}

route *Header::getRout( ) const {
    return rout;
}

void Header::setRout( route *rout )
{
    Header::rout = rout;
}

std::string &Header::getReminder( ) {
    return reminder;
}

void Header::setReminder( const std::string &reminder ) {
    Header::reminder = reminder;
}

bool Header::isEmptyLine( ) const {
    return empty_line;
}

void Header::setEmptyLine( bool emptyLine ) {
    empty_line = emptyLine;
}

void Header::setClient( int client ) {
    Header::client = client;
}

bool Header::isBodyEnd( ) const {
    return body_end;
}

void Header::setBodyEnd( bool bodyEnd ) {
    body_end = bodyEnd;
}

char **Header::env_to_char( )
{
    char                                **ret = (char **)malloc((env.size() + 1) * sizeof(char *));
    std::list<std::string>::iterator    it(env.begin());
    std::list<std::string>::iterator    ite(env.end());
    size_t                              i = 0;

    if (!ret)
        throw std::exception();
    while (it != ite)
    {
        ret[i] = strdup(const_cast<char *>(it->c_str()));
        if (!ret[i++])
            throw std::exception();
        it++;
    }
    ret[i] = NULL;
    return ret;
}

int Header::getReceiveFile( ) const {
    return receive_file;
}

void Header::setReceiveFile( int receiveFile ) {
    receive_file = receiveFile;
}

void Header::check_rout( )
{
    std::list<std::string>::const_iterator it((rout->get_http_methods().begin()));
    while (it != rout->get_http_methods().end())
    {
        if (*it == getMethod())
            break;
        it++;
    }
    if (it == rout->get_http_methods().end())
    {
        if (serv->getAllow().first != extension || serv->getAllow().second != getMethod())
            setFile(serv->exception_processing(405, *this));
    }
}

bool Header::operator==( const Header &head ) const {
    return real_path_to_file == head.getRealPathToFile();
}

Header &Header::operator=(Header const & src) {
    client_now_in_queue = src.client_now_in_queue;
    body_end = src.body_end;
    client = src.client;
    empty_line = src.empty_line;
    reminder = src.reminder;
    extension = src.extension;
    rout = src.rout;
    real_path_to_file = src.real_path_to_file;
    serv = src.serv;
    error = src.error;
    BodySize = src.BodySize;
    file = src.file;
    receive_file = src.receive_file;
    Port = src.Port;
    Request = src.Request;
    Response = src.Response;
    Method = src.Method;
    Http = src.Http;
    Accept_Charsets = src.Accept_Charsets;
    Accept_Language = src.Accept_Language;
    Allow = src.Allow;
    Authorization = src.Authorization;
    Content_Language = src.Content_Language;
    Content_Length = src.Content_Length;
    Content_Location = src.Content_Location;
    Content_Type = src.Content_Type;
    Date = src.Date;
    Host = src.Host;
    Location = src.Location;
    Referer = src.Referer;
    Retry_after = src.Retry_after;
    Server = src.Server;
    Transfer_Encoding = src.Transfer_Encoding;
    User_Agent = src.User_Agent;
    WWW_Authenticate = src.WWW_Authenticate;
    is_cgi = src.is_cgi;
    Pid = src.Pid;
    host_header_response = src.host_header_response;
    query = src.query;
    array = src.array;
    ip_addr = src.ip_addr;
    ad = src.ad;
    adlen = src.adlen;
    env = src.env;
    permission = src.permission;
    buffer = src.buffer;
    return *this;
}

Header::Header( const Header & src ) {
    *this = src;
}

bool Header::isClientNowInQueue( ) const {
    return client_now_in_queue;
}

void Header::setClientNowInQueue( bool clientNowInQueue ) {
    client_now_in_queue = clientNowInQueue;
}

bool Header::isPermission( ) const {
    return permission;
}

void Header::setPermission( bool permission ) {
    Header::permission = permission;
}

std::string &Header::getBuffer( ) {
    return buffer;
}

void Header::setBuffer( const std::string &buffer ) {
    Header::buffer = buffer;
}
