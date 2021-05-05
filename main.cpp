#include "header.h"

int main( )
{
    int     fd;
    struct sockaddr_in addr;
    std::string     ip = "192.168.1.45";
//    struct sockaddr addr2;

//    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
        error_exit("bind error");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1024);
//    addr.sin_addr.s_addr = htonl(ip.c_str());
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (-1 == bind(fd, (struct sockaddr*)&addr, sizeof(addr)))
        error_exit("fail to bind IP");
}
