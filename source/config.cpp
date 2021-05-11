<<<<<<< HEAD
//
// Created by Pamula File on 5/8/21.
//
#include "../header.h"

server    default_config(void)
{
    char buf[150];

    std::string root = getcwd(buf, sizeof(buf));
    server c("10.21.22.31", 1024, std::map<int, std::string>(), \
            std::list<route>(1, route("/", root+"/content/", std::list<std::string>())));
//    std::cout << "root: " << (*(c.get_routes().begin())).get_root()  << std::endl;
    return c;
}
=======
#include "config.hpp"

config::config() : servers() {

}

config::config(const config &other) : servers(other.servers) {

}

config &config::operator=(const config &other) {
    servers = other.servers;
    return *this;
}

config::~config() {

}

void config::add_server(const server &server_) {
    servers.push_back(server_);
}

std::ostream &operator<<(std::ostream &o, const config &conf) {
	o << "config:" << std::endl;
	for (config::ServersType::const_iterator it = conf.servers.begin();
		it != conf.servers.end(); ++it) {
		o << *it;
	}
	return o;
}
>>>>>>> 5fb5f9ed354185f247e749814988ec721690c497
