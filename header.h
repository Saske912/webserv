//
// Created by Pamula File on 5/5/21.
//

#ifndef HEADER_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <cstdio>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <set>
#include "source/t_config.hpp"

#define QUEUE 16
#define TVS 320000
#define TVMS 0
#define BUFSIZE 1024

typedef struct  s_data
{
    fd_set  read;
    fd_set  write;
    fd_set  emerg;
    char    buf[BUFSIZE];
    int                 max_d;
    long int            rd;
    int                 ret;
}               t_data;

typedef struct  s_serv
{
    int                 host;
    sockaddr_in         addr;
    int                 opt;
}               t_serv;

typedef struct  s_client
{
        int                 client;
        sockaddr            ad;
        socklen_t           adlen;
}               t_client;

void        error_exit(const std::string& str);
t_data      init_fd_sets(void);
sockaddr_in init_host_addr(void);
timeval     init_timevals(void);
t_serv      init_serv(void);
void        loop(timeval &tv, t_serv &serv, t_data &t);
t_config    default_config(void);

#define HEADER_H

#endif //HEADER_H
