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
        sockaddr_in            ad;
        socklen_t           adlen;
}               t_client;

typedef struct	s_write
{
		Header head;
		std::string addr;
		std::string reminder;
		int fd;
		int count;
		int bytes;
		int ct;
		bool flag;
        bool first_line;
        bool head_readed;
		bool eshe_odin_flag;
		std::string send_error;
}				t_write;

void        error_exit(const std::string& str);
void		init_fd_sets(t_data &);
sockaddr_in init_host_addr(void);
timeval     init_timevals(void);
t_serv      init_serv(void);
void        loop(timeval &tv, t_serv &serv, t_data &t, std::list<server> &conf);
server      default_config(void);
void        erase(std::list<t_write>::iterator &it, int fd, bool flag);
void        resetIt(std::list<t_write>::iterator &it);
int         send_protected(std::string const & str, std::list<t_write>::iterator &it, std::string str2);
bool        file_available(const std::string& request);
std::string     split_buffer(std::string &buffer, Header &head);
std::string     get_http_line(std::string &buffer, Header &head);
std::string     parse_request(std::string string, Header &head);

#endif //HEADER_H
