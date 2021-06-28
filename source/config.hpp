#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <list>
#include "server.hpp"
#include "header.hpp"
#include <semaphore.h>

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

    int                 opt;
    int                 ret;
    fd_set              conf_set;
    std::list<int>      sockets;
    timeval             tv;
private:
    std::list<std::string>  env;
    std::list<server>       servers;
};

std::ostream &operator<<(std::ostream &o, const config &conf);

#endif
