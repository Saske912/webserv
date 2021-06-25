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

    int                 opt;
    char                **env;
    int                 ret;
private:
    std::list<server>   servers;
public:
    std::list<server> getServers( ) const;

    void setServers( const std::list<server> &servers );
};

std::ostream &operator<<(std::ostream &o, const config &conf);

#endif
