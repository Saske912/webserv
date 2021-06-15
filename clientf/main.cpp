//#include "../header.h"
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main( )
{
    struct sockaddr_in  addr;
    socklen_t           addrlen;
//    struct timeval      tv;
    int                 sock;
    std::string     ip = "127.0.0.1";

    sock = socket(AF_INET, SOCK_STREAM, 0);
//    tv.tv_sec = 3;
//    tv.tv_usec = 500000;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1024);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addrlen = sizeof(addr);
    if (connect(sock, reinterpret_cast<sockaddr *>(&addr), addrlen) == -1)
//        error_exit("fail to Connect");
        exit(1);
    std::string tmp = "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n";
    char buf[327888];
    int ret;
    while ( true )
    {
        send(sock, tmp.c_str(), tmp.length(), 0);
        sleep(2);
        ret = recv(sock, buf, 32769, 0);
        buf[ret] = 0;
        std::cout << "buf: " << buf  << std::endl;
    }
//    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}
