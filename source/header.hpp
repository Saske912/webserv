#ifndef HEADER_HPP
#define HEADER_HPP
#include <list>
#include <map>
#include <string>
#include <sys/socket.h>
#include <libc.h>
#include <utility>
#include "server.hpp"

class config;
class server;

class Header
{
    typedef void (Header::*Func)(std::string const &);
private:
    Header();
    bool        permission;
    bool        client_now_in_queue;
    bool        body_end;
    int         client;
    bool        empty_line;
    std::string reminder;
    std::string extension;
    route       *rout;
    std::string real_path_to_file;
    server      *serv;
    int         error;
    int         BodySize;
    int         file;
    int         receive_file;
    std::list<std::string> env;
    unsigned int Port;
    std::string Request;
    std::string Response;
    std::string Method;
    std::string Http;
    std::string Accept_Charsets;
    std::string Accept_Language;
    std::string Allow;
    std::string Authorization;
    std::string Content_Language;
    std::string Content_Length;
    std::string Content_Location;
    std::string Content_Type;
    std::string Date;
    std::string Host;
    const static std::string Last_Modified;
    std::string Location;
    std::string Referer;
    std::string Retry_after;
    std::string Server;
    std::string Transfer_Encoding;
    std::string User_Agent;
    std::string WWW_Authenticate;
    bool        is_cgi;
    pid_t       Pid;
    std::string host_header_response;
    std::string query;
public:
    bool isPermission( ) const;
    bool isClientNowInQueue( ) const;
    void setClientNowInQueue( bool clientNowInQueue );
    bool operator==(Header const & head) const;
    Header & operator=(Header const & src);
    Header(Header const & src);
    int getReceiveFile( ) const;
    void setReceiveFile( int receiveFile );
    char **env_to_char();
    bool isBodyEnd( ) const;
    void setBodyEnd( bool bodyEnd );
    void setClient( int client );
    bool isEmptyLine( ) const;
    void setEmptyLine( bool emptyLine );
    const std::string &getReminder( ) const;
    void setReminder( const std::string &reminder );
    route *getRout( ) const;
    void setRout( route *rout );
    Header( server &serv, const std::list<std::string> &env);
    void setter( const std::string &line, server &serv );
    void    http( std::string const & str);
    void    host(std::string const & string);
    void    referer(std::string const & string);
    void    accept(std::string const & string);
    void    cgi_env();
    std::string &getAccept_Charsets();
    std::string &getAccept_Language();
    std::string &getAllow();
    std::string &getAuthorization();
    std::string &getContent_Language();
    std::string &getContent_Length();
    std::string &getContent_Location();
    std::string &getContent_Type();
    std::string &getDate();
    std::string &getHost();
    std::string &getHttp();
    std::string const &getLast_Modified();
//    std::string &getLocation();
    std::string &getMethod();
    std::string &getReferer();
    std::string &getResponse();
    std::string getRequest();
    std::string &getRetry_after();
    std::string &getServer();
    std::string &getTransfer_Encoding();
    std::string &getUser_Agent();
    std::string &getWWW_Authenticate();
    bool &getIsCgi();
    const std::list<std::string> & getEnv();
    std::string getEnvValue(char const *);
    void addEnv( const std::string& str );
    void showEnv();
    unsigned int getPort() const;
    int getFile();
    pid_t getPid();
    int	getBodySize() const;
    void setBodySize(int);
    void setPid(pid_t const &);
    void setFile( int const &);
    void setEnv( const std::list<std::string> &env);
    void setPort(unsigned int const &);
    void setAccept_Charsets(std::string const &);
    void setAccept_Language(std::string const &);
    void setAllow(std::string const &);
    void setAuthorization(std::string const &);
    void setContent_Language(std::string const &);
    void setContent_Length(std::string const &);
    void setContent_Location(std::string const &);
    void setContent_Type(std::string const &);
    void setDate(std::string const &);
    void setHost(std::string const &);
    void setHttp(std::string const &);
//    void setLocation(std::string const &);
    void setMethod(std::string const &);
    void setReferer(std::string const &);
    void setRequest(std::string const &);
    void setResponse(std::string const &);
    void setRetry_after(std::string const &);
    void setServer(std::string const &);
    void setTransfer_Encoding(std::string const &);
    void setUser_Agent(std::string const &);
    void setWWW_Authenticate(std::string const &);
    void setIsCgi(bool status);
    void eraseStruct();
    int getClient( ) const;
    void setServ( server *serv );
    int getError( ) const;
    void setError( int error );
    server *getServ( ) const;
    const std::string &getQuery( ) const;
    void setQuery( const std::string &query );
    const std::string &getHostHeaderResponse( ) const;
    void setHostHeaderResponse( const std::string &host, unsigned int port);
    const std::string &getRealPathToFile( ) const;
    void setRealPathToFile( const std::string &realPathToFile );
    const std::string &getExtension( ) const;
    void setExtension( const std::string &filename );
    static std::list<std::string>   current_files_in_work;
    std::map<std::string, Func>     array;
    std::string                     ip_addr;
    sockaddr_in                     ad;
    socklen_t                       adlen;
protected:
    void    put_string( std::string );
    void    check_rout();
};

#endif
