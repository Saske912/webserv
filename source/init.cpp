//
// Created by Pamula File on 5/6/21.
//
#include "header.h"

sockaddr_in init_host_addr( unsigned int port )
{
    sockaddr_in         addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    return (addr);
}

void init_serv( config &serv )
{
    int     sock;

    std::list<server>::iterator it = serv.getServers().begin();
    while (it != serv.getServers().end())
    {
        it->addr = (init_host_addr(it->get_port()));
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1)
            error_exit("socket error");
        it->setHostRaw(sock);
        serv.opt = 1;
        setsockopt( it->getHostSock( ), SOL_SOCKET, SO_REUSEADDR, &serv.opt, sizeof(serv.opt));
        if (serv.opt != 1)
            error_exit("SO_REUSEADDR");
        serv.opt = BUFSIZE;
        setsockopt( it->getHostSock( ), SOL_SOCKET, SO_RCVBUF, &serv.opt, sizeof(serv.opt));
        if (serv.opt != 1)
            error_exit("SO_RCVBUF");
        if ( bind( it->getHostSock( ), reinterpret_cast<sockaddr *>(&it->addr), sizeof(it->addr)) == -1)
            error_exit("fail to bind IP");
        if ( listen( it->getHostSock( ), QUEUE) == -1)
            error_exit("listening error");
        it++;
    }
}
