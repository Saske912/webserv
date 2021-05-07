//
// Created by Pamula File on 5/5/21.
//
#include "../header.h"
#include <set>

int main(void)
{
    int                 host;
    sockaddr_in         addr = init_host_addr();
    int client = 0;
    sockaddr            ad;
    socklen_t           adlen;
    t_data              t;
    char                buf[BUFSIZE];
    long int            rd;
    timeval             tv = init_timevals();
    std::set<int>      set;
    int                 ret;
//    int                 max_d;

    if ((host = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        error_exit("bind error");
    if (bind(host, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
        error_exit("fail to bind IP");
    if (listen(host, QUEUE) == -1)
        error_exit("listening error");
    while (true)
    {
        t = init_fd_sets();
        set.insert(host);
        FD_SET(host, &t.read);
        std::set<int>::iterator it = set.begin();
        while ( it != set.end())
        {
            FD_SET(*it, &t.read);
            FD_SET(*it, &t.write);             //if we have data to send
            it++;
        }
        if ((ret = select(*set.rbegin() + 1, &t.read, &t.write, NULL, &tv)) < 1)
        {
            if (errno != EINTR)
                error_exit("error in select");
            else
            {
                //signal income
            }
            continue;
        }
        if (!ret)
        {
            //timeout
            continue;
        }
        if ( FD_ISSET(host, &t.read))
        {
            if (( client = accept( host, &ad, &adlen)) == -1)
                error_exit("fail to accept Client");
            fcntl( client, F_SETFL, O_NONBLOCK);
            set.insert(client);
        }
        it = set.begin();
        while (it != set.end())
        {
            fcntl( client, F_SETFL, O_NONBLOCK);
            if ( FD_ISSET(*it, &t.read))
            {
               rd = recv( client, buf, 1024, 0);
                buf[rd] = 0;
                std::cout << buf  << std::endl;
                if (rd == -1)
                {
                    std::cout << "error here"  << std::endl;
                }
//                set.erase(client);
                std::cout << std::endl;
            }
            if ( FD_ISSET(*it, &t.write))
            {
                send( client, "HTTP/1.1 200 OK\n", 16, 0);
            }
            it++;
        }
//        set.clear();
    }
    return (0);
}

