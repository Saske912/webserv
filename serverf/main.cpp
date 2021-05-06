//
// Created by Pamula File on 5/5/21.
//
#include "../header.h"

int main(void)
{
    int                 fd;
    struct sockaddr_in  addr;
    int                 ret;
    struct sockaddr     ad;
    socklen_t           adlen;

//    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_TCP);
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        error_exit("bind error");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1024);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    fd_set fdset;
    FD_ZERO(&fdset);
    if (bind(fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
        error_exit("fail to bind IP");
    if (listen(fd, QUEUE) == -1)
        error_exit("listening error");
    while (true)
    {
        if ((ret = accept(fd, &ad, &adlen)) == -1)
            error_exit("fail to accept Client");
        if (fork() == 0)
        {
            close(fd);
            FD_SET(ret, &fdset);
            char buf[1025];
            long int rd;
            timeval tv = {};
            tv.tv_usec = 0;
            tv.tv_sec = 15;
            fcntl(ret, F_SETFL, O_NONBLOCK);
            select(40, &fdset, 0, 0, &tv);
            while ((rd = recv(ret, buf, 1024, 0)) > 0)
            {
                buf[rd] = 0;
                std::cout << buf;
            }
            std::cout << std::endl;
            std::cout << "connected: " << ret << std::endl;
            send(ret, "HTTP/1.1 200 OK\n", 16,0);
            exit(0);
        }
        close(ret);
        while ( wait4(-1, NULL, WNOHANG, NULL)>0)
        {}
    }
//    shutdown(fd, SHUT_RDWR);
    return (0);
}

