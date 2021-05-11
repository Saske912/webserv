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
