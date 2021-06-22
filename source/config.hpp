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
    server * find_server( std::string const & host_addr, unsigned int port);
    void add_server(const server &server_);

    int                 host;
    sockaddr_in         addr;
    int                 opt;
    char                **env;
    int                 max_d;
    int                 ret;
    fd_set              read;
    timeval             tv;
private:
    std::list<server>   servers;
public:
    std::list<server> getServers( ) const;

    void setServers( const std::list<server> &servers );
};

std::ostream &operator<<(std::ostream &o, const config &conf);

#endif
