//
// Created by Pamula File on 5/6/21.
//
#include "header.h"

sockaddr_in init_host_addr(void)
{
    sockaddr_in         addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1024);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    return (addr);
}

void init_serv( config &serv )
{

    serv.addr = init_host_addr();
    if ((serv.host = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        error_exit("socket error");
    serv.opt = 1;
    setsockopt(serv.host, SOL_SOCKET, SO_REUSEADDR, &serv.opt, sizeof(serv.opt));
    int    buf = BUFSIZE;
    setsockopt(serv.host, SOL_SOCKET, SO_RCVBUF, &buf, sizeof(serv.opt));
    if (bind(serv.host, reinterpret_cast<sockaddr *>(&serv.addr), sizeof(serv.addr)) == -1)
        error_exit("fail to bind IP");
    if (listen(serv.host, QUEUE) == -1)
        error_exit("listening error");
}
