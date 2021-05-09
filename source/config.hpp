#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <list>
#include "server.hpp"

class config {
public:
    config();
    config(const config& other);
    config &operator=(const config &other);
    ~config();

    void add_server(const server &server_);

    std::list<server> servers;
};

#endif
