#include "config.hpp"
#include "header.h"

config::config() : servers() {
    tv.tv_usec = TVMS;
    tv.tv_sec = TVS;
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

server config::find_server( const std::string &host_addr, unsigned int port ) {
    std::list<server>::iterator it = servers.begin( );
    while (it != servers.end())
    {
        if (it->get_host() == host_addr and it->get_port() == port)
        {
            return *it;
        }
        it++;
    }
    return *servers.begin();
}

std::ostream &operator<<(std::ostream &o, const config &conf) {
	o << "config:" << std::endl;
	for (config::ServersType::const_iterator it = conf.servers.begin();
		it != conf.servers.end(); ++it) {
		o << *it;
	}
	return o;
}
