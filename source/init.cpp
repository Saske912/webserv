//
// Created by Pamula File on 5/6/21.
//
#include "../header.h"

t_data    init_fd_sets(void)
{
    t_data  ret;

    FD_ZERO(&ret.read);
    FD_ZERO(&ret.write);
    FD_ZERO(&ret.emerg);
    return ret;
}

sockaddr_in init_host_addr(void)
{
    sockaddr_in         addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1024);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    return (addr);
}

timeval init_timevals(void)
{
    timeval tv;

    tv.tv_usec = TVMS;
    tv.tv_sec = TVS;
    return  tv;
}