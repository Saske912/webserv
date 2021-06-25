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
#include <iomanip>

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
#define END "\r\n"

void        error_exit(const std::string& str);
sockaddr_in init_host_addr( unsigned int port );
timeval     init_timevals();
void            init_serv( config &config );
void            loop(config &conf);
server      default_config();
void            erase( std::string const & realPathToFile, Header &head );
//void        resetIt(std::list<Header>::iterator &it);
int             send_protected( std::string str, Header &head );
bool            file_available(const std::string& request);
std::string split_buffer( std::string &buffer, Header &head, server &serv );
std::string get_http_line( std::string &buffer, Header &head, server &serv );
std::string parse_request( const std::string &string, Header &head, server &serv );
void response( std::list<Header>::iterator &it );

#endif //HEADER_H
