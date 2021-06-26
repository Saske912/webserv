#include "config.hpp"
#include "header.h"

config::config() : servers() {
    tv.tv_usec = TVMS;
    tv.tv_sec = TVS;
}

config::config(const config &other) {
    *this = other;
}

config &config::operator=(const config &other) {
    servers = other.servers;
    tv.tv_sec = other.tv.tv_sec;
    tv.tv_usec = other.tv.tv_usec;
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

std::list<server> &config::getServers( ) {
    return servers;
}

void config::setServers( const std::list<server> &servers ) {
    config::servers = servers;
}

const std::list<std::string> & config::getEnv( ) const
{
    return env;
}

void config::setEnv( const std::list<std::string> &env ) {
    config::env = env;
}

std::ostream &operator<<(std::ostream &o, config &conf) {
	o << "config:" << std::endl;
	for (config::ServersType::iterator it = conf.getServers().begin();
		it != conf.getServers().end(); ++it) {
		o << *it;
	}
	return o;
}
