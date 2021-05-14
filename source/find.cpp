//
// Created by Pamula File on 5/10/21.
//
#include "../header.h"

server  find_server(std::list<server> serv, std::string const & host, unsigned int port)
{
    std::list<server>::iterator it = serv.begin( );
    while (it != serv.end())
    {
//        std::cout << it->get_host() << " " << it->get_port()  << std::endl;
        if (it->get_host() == host and it->get_port() == port)
        {
            return *it;
        }
        it++;
    }
    return *serv.begin();
//    throw std::exception();
}