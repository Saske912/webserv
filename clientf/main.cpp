#include "../header.h"

int main( )
{
    struct sockaddr_in  addr;
    socklen_t           addrlen;
//    struct timeval      tv;
    int                 sock;
    std::string     ip = "10.21.21.84";

    sock = socket(AF_INET, SOCK_STREAM, 0);
//    tv.tv_sec = 3;
//    tv.tv_usec = 500000;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1024);
    addr.sin_addr.s_addr = htonl(inet_addr(ip.c_str()));
    addrlen = sizeof(addr);
    if (connect(sock, reinterpret_cast<sockaddr *>(&addr), addrlen) == -1)
        error_exit("fail to Connect");
//    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}
