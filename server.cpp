//
// Created by Pamula File on 5/5/21.
//
#include "header.h"

int server(sockaddr *ad, socklen_t *adlen)
{
    int                 fd;
    struct sockaddr_in  addr;
    int                 ret;
//    std::string     ip = "192.168.1.45";
//    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
        error_exit("bind error");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1024);
//    addr.sin_addr.s_addr = htonl(ip.c_str());
//    addr.sin_addr.s_addr = htonl(ip.c_str());
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
        error_exit("fail to bind IP");
    if (listen(fd, QUEUE) == -1)
        error_exit("listening error");
    if ((ret = accept(fd, ad, adlen)) == -1)
        error_exit("fail to accept Client");
    return (ret);
}
