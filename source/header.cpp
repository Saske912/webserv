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
	if (Env)
		ft_doublefree(Env);
	Port = 0;
	Env = 0;
	Fd = 0;
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
	Content_Language = "Content-Language: " + str;
}

void Header::setContent_Length(std::string const &str)
{
	Content_Length = str;
}

void Header::setContent_Location(std::string const &str)
{
	Content_Location = str;
}

void Header::setContent_Type(std::string const &str)
{
	Content_Type = "Content-Type: " + str;
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
	Response = str;
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

void Header::setEnv(char **env)
{
	if (Env)
		ft_doublefree(Env);
	Env = ft_doublecpy(env);
}

void Header::setFd(int const &fd)
{
	Fd = fd;
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

int	Header::getBodySize()
{
	return BodySize;
}

pid_t Header::getPid()
{
	return Pid;
}

char **Header::getEnv()
{
	return Env;
}

int Header::getFd()
{
	return Fd;
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
	int i = 0;
	char *tmp;

	while (Env[i])
	{
		if ((strstr(Env[i], str) == Env[i]))
		{
			tmp = Env[i] + strlen(str);
			return std::string(tmp);
		}
		++i;
	}
	return std::string();
}

void Header::addEnv(const char *str)
{
	int i = 0;
	char *tmp;

	tmp = strdup(std::string(str, 0, strchr(str, '=') - str).c_str());
	while (Env[i])
	{
		if (strstr(Env[i], tmp))
		{
			free(Env[i]);
			Env[i] = strdup(str);
			free(tmp);
			return ;
		}
		++i;
	}
	free(tmp);
	char **nu = (char **)malloc(sizeof(char *) * (i + 2));
	i = -1;
	while (Env[++i])
		nu[i] = strdup(Env[i]);
	nu[i++] = strdup(str);
	nu[i] = 0;
	ft_doublefree(Env);
	Env = nu;
}


void Header::showEnv()
{
	int i = -1;
	while (Env[++i])
		printf("%s\n", Env[i]);
}

Header::Header()
{
}

void Header::setter( const std::string &line )
{
    if (line.empty())
    {
        cgi_env();
        empty_line = true;
        return ;
    }
    for (std::map<std::string, Func>::iterator it(array.begin()); it != array.end();)
    {
        if (line.find(it->first) != std::string::npos)
            return (this->*array[it->first])(line);
        ++it;
    }
}

void Header::http( const std::string &string )
{
    setDate(get_current_date());
    setMethod(std::string( string, 0, string.find( ' ' )));
    setRequest(std::string( string, string.find( ' ' ) + 1, string.rfind( ' ' )));
    setHttp(std::string( string, string.find(HTTP) + strlen( HTTP )));
}

void Header::host( const std::string &string )
{
    setHost(std::string(string, string.find(' ') + 1, string.find(':')));
    setPort(atoi(std::string(string, string.find(':') + 1).c_str()));
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
//    if (std::string(getEnvValue("AUTH_TYPE=")) == "BASIC" || std::string(getEnvValue("AUTH_TYPE=")) == "DIGEST")
//    {
//        tmp = strchr(tmp, ' ') + 1;
//        addEnv((char *)("REMOTE_USER=" + std::string(tmp, 0, strchr(tmp, ':') - tmp)).c_str());
//        tmp = strchr(tmp, ':') + 1;
//        head.addEnv((char *)("REMOTE_IDENT=" + std::string(tmp)).c_str());
//    }
}

Header::Header( config &serv )
{
    int bufer = BUFSIZE;

    array.insert(std::pair<std::string, Func>(HTTP, &Header::http));
    array.insert(std::pair<std::string, Func>(ACEPT_LANG, &Header::setContent_Language));
    array.insert(std::pair<std::string, Func>(HOST, &Header::host));
    array.insert(std::pair<std::string, Func>(REFERER, &Header::referer));
    array.insert(std::pair<std::string, Func>(ACCEPT, &Header::accept));
    array.insert(std::pair<std::string, Func>(TRANS_ENC, &Header::setTransfer_Encoding));
    array.insert(std::pair<std::string, Func>(AUTH, &Header::setAuthorization));
    Env = ft_doublecpy(serv.env);
    Fd = 0;
    is_cgi = false;
    Pid = 0;
    BodySize = 0;
    empty_line = false;
    body_end = false;
    if (( client = ::accept( serv.host, reinterpret_cast<sockaddr *>(&ad), &adlen)) == -1)
    {
        perror("accept");
    }
    fcntl(client, F_SETFL, O_NONBLOCK);
    serv.opt = 1;
    setsockopt( client, SOL_SOCKET, SO_NOSIGPIPE, reinterpret_cast<const void *>(serv.opt), sizeof(serv.opt));
    setsockopt(client, SOL_SOCKET, SO_SNDBUF, &bufer, sizeof(bufer));
    ip_addr = ttostr(ad.sin_addr.s_addr & 255) + '.' +
            ttostr(ad.sin_addr.s_addr >> 8 & 255) + '.' +
            ttostr(ad.sin_addr.s_addr >> 16 & 255) + '.' +
            ttostr(ad.sin_addr.s_addr >> 24);
}

int Header::getClient( ) const {
    return client;
}
