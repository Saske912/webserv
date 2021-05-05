#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

int main( )
{
    int     fd;
    struct sockaddr_in addr;
    std::string     inet_addr = "192.168.1.45";
    struct sockaddr addr2;

//    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1024);
//    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (!inet_aton(inet_addr.c_str(), &(addr.sin_addr)))
    {
        std::cout << "invalid IP" << std::endl;
        exit(1);
    }
    if (-1 == bind(fd, (struct sockaddr*)&addr, sizeof(addr)))
    {
        perror("fail to bind IP");
        exit(1);
    }
}
