#include "config.hpp"
#include "header.h"

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

server *config::find_server( unsigned int port, Header &head ) {
    std::list<server>::iterator it = servers.begin( );
    if (head.getHost() == "localhost")
        head.setHost("127.0.0.1");
    while (it != servers.end())
    {
        if (it->get_host() == head.getHost() and it->get_port() == port)
            return &(*it);
        it++;
    }
    return NULL;
}

std::list<server> config::getServers( ) const {
    return servers;
}

void config::setServers( const std::list<server> &servers ) {
    config::servers = servers;
}

std::ostream &operator<<(std::ostream &o, const config &conf) {
	o << "config:" << std::endl;
	for (config::ServersType::const_iterator it = conf.getServers().begin();
		it != conf.getServers().end(); ++it) {
		o << *it;
	}
	return o;
}
