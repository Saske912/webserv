//
// Created by Pamula File on 5/5/21.
//

#ifndef HEADER_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#define QUEUE 16
#define MAXFD 1024
#define TVS 320000
#define TVMS 0
#define BUFSIZE 1024

typedef struct  s_data
{
    fd_set  read;
    fd_set  write;
    fd_set  emerg;
}               t_data;

void        error_exit(const std::string& str);
t_data      init_fd_sets(void);
sockaddr_in init_host_addr(void);
timeval     init_timevals(void);
void        add_fd_to_set(int fd, t_data *t);

#define HEADER_H

#endif //HEADER_H
