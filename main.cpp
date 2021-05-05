#include "header.h"

int main( )
{
    struct sockaddr_in  addr;
    socklen_t           addrlen;
    struct timeval      tv;
    int                 fd;
    int                 sock;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    tv.tv_sec = 3;
    tv.tv_usec = 500000;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12024);
    addr.sin_addr.s_addr = INADDR_ANY;
    addrlen = sizeof(addr);
    if (connect(sock, reinterpret_cast<sockaddr *>(&addr), addrlen) == -1)
        error_exit("fail to Connect");
    fd = server(reinterpret_cast<sockaddr *>(&addr), &addrlen);
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}
