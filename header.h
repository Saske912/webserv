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
#define QUEUE 16

void error_exit(const std::string& str);
int server(sockaddr *addr, socklen_t *addrlen);

#define HEADER_H

#endif //HEADER_H
