#include "../wpersimm.h"
#include "header.hpp"

std::string const Header::Last_Modified = "Last_Modified: " + get_current_date();

Header::Header()
{
	Env = 0;
}

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
	Host.erase();
	Port = 0;
	Env = 0;
}

void Header::setAccept_Charsets(std::string const &str)
{
	Accept_Charsets = str;
}

void Header::setAccept_Language(std::string const &str)
{
	Accept_Language = str;
}

void Header::setAllow(std::string const &str)
{
	Allow = str;
}

void Header::setAuthorization(std::string const &str)
{
	Authorization = str;
}

void Header::setContent_Language(std::string const &str)
{
	Content_Language = str;
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
	Content_Type = str;
}

void Header::setDate(std::string const &str)
{
	Date = str;
}

void Header::setHttp(std::string const &str)
{
	Http = str;
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
	Transfer_Encoding = str;
}

void Header::setUser_Agent(std::string const &str)
{
	User_Agent = str;
}

void Header::setWWW_Authenticate(std::string const &str)
{
	WWW_Authenticate = str;
}

void Header::setEnv(char **env)
{
	if (Env)
		ft_doublefree(Env);
	Env = ft_doublecpy(env);
}

void Header::setPort(unsigned int  const &port)
{
	Port = port;
}

char **Header::getEnv()
{
	return Env;
}

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

std::string &Header::getRequest()
{
	return Request;
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


char **Header::addEnv(char *str, char c)
{
	char **split = ft_split(str, c);
	int i = 0;
	int j = 0;

	printf("INCOMIG STRING = %s\n", str);	
	while (Env[i])
		++i;
	while (split[j])
		++j;
	char **nu = (char **)malloc(sizeof(char *) * (i + j + 1));
	i = -1;
	while (Env[++i])
		nu[i] = strdup(Env[i]);
	j = -1;
	while (split[++j])
		nu[i + j] = split[j];
	nu[i + j] = 0;
	ft_doublefree(Env);
	ft_doublefree(split);
	Env = nu;
	i = -1;
	printf("------------------------------------------------\n");
	while (Env[++i])
		printf("%s\n", Env[i]);
	printf("------------------------------------------------\n");
	return (Env);
}
