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
    int                 max_d;

    if ((host = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        error_exit("bind error");
    int opt = 1;
    setsockopt(host, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(host, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
        error_exit("fail to bind IP");
    if (listen(host, QUEUE) == -1)
        error_exit("listening error");
    while (true)
    {
        t = init_fd_sets();
//        set.insert(host);
        FD_SET(host, &t.read);
        std::set<int>::iterator it = set.begin();
        while ( it != set.end())
        {
            FD_SET(*it, &t.read);
            FD_SET(*it, &t.write);             //if we have data to send
            it++;
        }
        if (set.empty())
        {
            max_d = host;
        }
        else
        {
            max_d = *set.rbegin() > host ? *set.rbegin() : host;
            if ((ret = select(max_d + 1, &t.read, &t.write, NULL, &tv)) < 1)
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
        }
        if ( FD_ISSET(host, &t.read))
        {
            if (( client = accept( host, &ad, &adlen)) == -1)
                error_exit("fail to accept Client");
            fcntl( client, F_SETFL, O_NONBLOCK);
            opt = 1;
            setsockopt(client, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
            set.insert(client);
        }
        it = set.begin();
        while (it != set.end())
        {
//            fcntl( *it, F_SETFL, O_NONBLOCK);
            adlen = sizeof(opt);
            getsockopt(*it, SOL_SOCKET, SO_KEEPALIVE, &opt, &adlen);
            if (!opt)
            {
                std::cout << "catch"  << std::endl;
                set.erase(it);
                continue;
            }
            if ( FD_ISSET(*it, &t.read))
            {
                while ((rd = recv( *it, buf, 1024, 0)) > 0)
                {
                    buf[rd] = 0;
                    std::cout << buf;
                }
                if (rd == 0)
                {
                    set.erase(*it);
                    break;
                }
            }
            if ( FD_ISSET(*it, &t.write))
            {
                send( (*it), "HTTP/1.1 200 OK\n", 16, 0);
            }
            it++;
        }
//        set.clear();
    }
    return (0);
}

