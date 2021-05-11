#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <list>
#include "server.hpp"

class config {
public:
	typedef std::list<server> ServersType;

    config();
    config(const config& other);
    config &operator=(const config &other);
    ~config();

    void add_server(const server &server_);

    ServersType servers;
};

std::ostream &operator<<(std::ostream &o, const config &conf);

#endif
