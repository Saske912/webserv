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
	env.clear();
	close(file);
    file = 0;
    error = 0;
    body_end = false;
    empty_line = false;
//	Fdr = 0;
	is_cgi = false;
	Pid = 0;
	BodySize = 0;
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

unsigned int Header::getPort()
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

void Header::initEnv()
{

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

void Header::addEnv(const char *str1)
{
	std::string str = std::string(str1);
	int len = str.find('=');
	str.erase(len, str.length() - len);

	std::list<std::string>::iterator it = env.begin();
	while (it != env.end())
	{
		if (it->find(str) == 0)
		{
			env.erase(it);
			break ;
		}
		++it;
	}
	env.push_back(str1);
}

void Header::put_string( std::string str ) {
    std::cout << str  << std::endl;
}

void Header::showEnv()
{
//	std::for_each(env.begin(), env.end(), std::mem_fun_ref(&Header::put_string));
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
    if (line.empty())
    {
        empty_line = true;
//        server *server = conf.find_server( getPort( ), *this );
//        if (!server)
//        {
//            setError(502);
//            setFile(conf.getServers().front().exception_processing(502, *this));
//            body_end = true;
//            return;
//        }
        setServ(&serv);
        cgi_env();
        serv.concat( *this );
        if (getMethod() == "GET" || getMethod() == "HEAD" || getError())
        {
            setFile(serv.descriptorForSend( *this ));
            body_end = true;
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

    setDate(get_current_date());
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

    addEnv((char *)("REQUEST_URI=" + getRequest()).c_str());
    addEnv((char *)("REQUEST_METHOD=" + getMethod()).c_str());
    addEnv((char *)("SERVER_PROTOCOL=" + getHttp()).c_str());
    addEnv((char *)("PATH_INFO=" + getRequest()).c_str());
    addEnv((char *)("PATH_TRANSLATED=" + getRequest()).c_str());
    str = getRequest();
    if ((pos = str.find('?')) != std::string::npos)
        addEnv((char *)(("QUERY_STRING=" + std::string(str, pos + 1)).c_str()));
    //
    addEnv((char *)("CONTENT_TYPE=" + std::string(getContent_Type(), 0, getContent_Type().find(','))).c_str());
    addEnv((char *)("AUTH_TYPE=" + std::string(getAuthorization(), 0, getAuthorization().find(' '))).c_str());
    addEnv((char *)"GATEWAY_INTERFACE=CGI/0.9");
    addEnv((char *)("REMOTE_ADDR=" + ip_addr).c_str());
    addEnv((char *)"SERVER_SOFTWARE=webserv/1.0 (Unix)");
    addEnv((char *)("PATH_INFO=" + getRequest()).c_str());
    addEnv((char *)"HTTP_X_SECRET_HEADER_FOR_TEST=1");
    addEnv(const_cast<char *>(("SERVER_NAME=" + getServ()->get_server_names().front()).c_str()));
    addEnv(const_cast<char *>(("SERVER_PORT=" + ttostr(static_cast<int>(getServ()->get_port()))).c_str()));
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
    Header::env = env;
    file = 0;
    error = 0;
    is_cgi = false;
    Pid = 0;
    BodySize = 0;
    empty_line = false;
    body_end = false;
    if (( client = ::accept( serv.getHostSock( ), reinterpret_cast<sockaddr *>(&ad), &adlen)) == -1)
    {
        perror("accept");
    }
    fcntl(client, F_SETFL, O_NONBLOCK);
    opt = 1;
//    setsockopt( client, SOL_SOCKET, SO_NOSIGPIPE, reinterpret_cast<const void *>(serv.opt), sizeof(serv.opt));
    setsockopt(client, SOL_SOCKET, SO_SNDBUF, &bufer, sizeof(bufer));
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
    if (::stat(realPathToFile.c_str(), &st) == -1)
    {
        if (errno == EACCES)
        {
            setError(403);
            setFile(serv->exception_processing(403, *this));
        }
        else
        {
            setError(404);
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
                real_path_to_file = rtrim(realPathToFile, "/") + "/" + rout->get_default_page();
        }
    }
    else
        real_path_to_file = realPathToFile;
    setExtension(real_path_to_file);
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
        {
            setError(405);
            file = serv->exception_processing(405, *this);
        }
    }
    Header::rout = rout;
}

const std::string &Header::getReminder( ) const {
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
    char                                **ret = (char **)malloc(env.size() + 1 * sizeof(char *));
    std::list<std::string>::iterator    it(env.begin());
    size_t                              i = 0;

    if (!ret)
        throw std::exception();
    while (it != env.end())
    {
        ret[i] = strdup(it->c_str());
        if (!ret[i++])
            throw std::exception();
    }
    ret[i] = NULL;
    return ret;
}
