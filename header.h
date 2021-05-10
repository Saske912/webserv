//
// Created by Pamula File on 5/5/21.
//

#ifndef HEADER_H

#include <iostream>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <cstdio>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <set>
#include "source/server.hpp"
#include <algorithm>
#include "pfile.h"
#include "wpersimm.h"
#include "itressa.h"

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
    char                **env;
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

typedef struct	s_write
{
	int fd;
	bool flag;
}				t_write;

void        error_exit(const std::string& str);
void		init_fd_sets(t_data &);
sockaddr_in init_host_addr(void);
timeval     init_timevals(void);
t_serv      init_serv(void);
void        loop(timeval &tv, t_serv &serv, t_data &t, std::list<server> &conf);
server    default_config(void);

#define HEADER_H

#endif //HEADER_H
