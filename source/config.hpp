#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <list>
#include "server.hpp"
#include "header.hpp"

class config {
public:
    typedef std::list<server> ServersType;
    config();
    config(const config& other);
    config &operator=(const config &other);
    ~config();
    server *find_server( unsigned int port, Header &head );
    void add_server(const server &server_);
    const std::list<std::string> & getEnv( ) const;
    void setEnv( const std::list<std::string> &env );
    std::list<server> &getServers( );
    void setServers( const std::list<server> &servers );
    void moveToWait( Header &head, std::list<Header> &_set );
    void moveFromWait(std::string const & rpf);
    std::list<std::queue<Header> > &getWait( );
    void setWait( const std::list<std::queue<Header> > &wait );
    server &get_serv(unsigned int port);

    int                 opt;
    int                 ret;
    fd_set              conf_set;
    std::list<int>      sockets;
    timeval             tv;
private:
    std::list<std::queue<Header> >      _wait;
    std::list<std::string>  env;
    std::list<server>       servers;
};

std::ostream &operator<<(std::ostream &o, const config &conf);

#endif
