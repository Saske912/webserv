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
    env = other.env;
    conf_set = other.conf_set;
    sockets = other.sockets;
    opt = other.opt;
    ret = other.ret;
    _wait = other._wait;
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

void config::moveToWait( Header &head, std::list<Header> &_set ) {
    std::list<std::queue<Header> >::iterator    it(_wait.begin());
    std::list<std::queue<Header> >::iterator    ite(_wait.end());
    std::list<Header>::iterator                 it_set(_set.begin());
    std::list<Header>::iterator                 ite_set(_set.end());
    std::queue<Header>                          temp;

    while (it != ite)
    {
        if (it->back().getRealPathToFile() == head.getRealPathToFile())
        {
            it->push(head);
            break;
        }
        it++;
    }
    if (it == ite)
    {
        temp.push(head);
        _wait.push_back(temp);
    }
    while (it_set != ite_set)
    {
        if (*it_set == head)
        {
            _set.erase(it_set);
            std::cerr << "client from server with port " << it_set->getServ()->get_port();
            std::cerr << " move to queue with path " << it_set->getRealPathToFile() << std::endl;
            return;
        }
        it_set++;
    }
    throw std::exception();
}

std::list<std::queue<Header> > &config::getWait( ) {
    return _wait;
}

void config::setWait( const std::list<std::queue<Header> > &wait ) {
    _wait = wait;
}

void config::moveFromWait( const std::string &rpf ) {
    std::list<std::queue<Header> >::iterator it(_wait.begin());
    std::list<std::queue<Header> >::iterator ite(_wait.end());
    std::list<server>::iterator it_serv(servers.begin());
    std::list<server>::iterator ite_serv(servers.end());

    while(it != ite)
    {
        if (it->back().getRealPathToFile() == rpf)
        {
            Header  tmp = it->front();
            while (it_serv != ite_serv)
            {
                if (*it_serv == it->front())
                {
                    std::cerr << "client from server with port " << it->front().getServ()->get_port();
                    std::cerr << " coming back for file " << rpf << std::endl;
                    it_serv->getSet().push_back(it->front());
                    break ;
                }
                it_serv++;
            }
            it->pop();
            break ;
        }
        it++;
    }
}
