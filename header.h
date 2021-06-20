//
// Created by Pamula File on 5/5/21.
//

#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
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
#include <dirent.h>
#include <list>
#include <map>
#include <vector>

#define QUEUE 16
#define TVS 320000
#define TVMS 0
#define BUFSIZE 1048576

#define ACEPT_LANG "Accept-Language: "
#define HTTP        "HTTP/"
#define CONT_LANG   "Content-Language: "
#define METHOD      "method"
#define REQUEST     "request"
#define HOST        "Host: "
#define REFERER     "Referer: "
#define ACCEPT      "Accept: "
#define TRANS_ENC   "Transfer-Encoding: "
#define AUTH "Authorization: "

void        error_exit(const std::string& str);
sockaddr_in init_host_addr();
timeval     init_timevals();
void init_serv( config &serv );
void        loop(config &conf);
server      default_config();
std::list<Header>::iterator& erase( std::list<Header>::iterator &it );
//void        resetIt(std::list<Header>::iterator &it);
int         send_protected( std::string const & str, std::list<Header>::iterator &it, std::string str2);
bool        file_available(const std::string& request);
std::string     split_buffer(std::string &buffer, Header &head, config &conf);
std::string     get_http_line(std::string &buffer, Header &head, config &conf);
std::string     parse_request(const std::string& string, Header &head, config &conf);
void            response( std::list<Header>::iterator &it, config &conf);

#endif //HEADER_H
