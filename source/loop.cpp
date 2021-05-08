//
// Created by Pamula File on 5/8/21.
//
#include "../header.h"


void    loop(timeval &tv, t_serv &serv, t_data &t)
{
    t_client            cli;
    std::set<int>       set;

    while (true)
    {
        t = init_fd_sets();
        FD_SET(serv.host, &t.read);
        std::set<int>::iterator it = set.begin();
        while ( it != set.end())
        {
            FD_SET(*it, &t.read);
            FD_SET(*it, &t.write);             //if we have data to send
            it++;
        }
        if (set.empty())
        {
            t.max_d = serv.host;
        }
        else
        {
            t.max_d = *set.rbegin() > serv.host ? *set.rbegin() : serv.host;
            if ((t.ret = select(t.max_d + 1, &t.read, &t.write, NULL, &tv)) < 1)
            {
                if (errno != EINTR)
                    error_exit("error in select");
                else
                {
                    //signal income
                    return;
                }
//                loop(tv, serv, t, cli, set);
                continue;
            }
            if (!t.ret)
            {
                //timeout
//                loop(tv, serv, t, cli, set);
                continue;
            }
        }
        if ( FD_ISSET(serv.host, &t.read))
        {
            if (( cli.client = accept( serv.host, &cli.ad, &cli.adlen)) == -1)
                error_exit("fail to accept Client");
            fcntl( cli.client, F_SETFL, O_NONBLOCK);
            serv.opt = 1;
            setsockopt(cli.client, SOL_SOCKET, SO_NOSIGPIPE, &serv.opt, sizeof(serv.opt));
            set.insert(cli.client);
        }
        it = set.begin();
        while (it != set.end())
        {
            if (!serv.opt)
            {
                std::cout << "catch"  << std::endl;
                set.erase(it);
                continue;
            }
            if ( FD_ISSET(*it, &t.read))
            {
                while ((t.rd = recv( *it, t.buf, 1024, 0)) > 0)
                {
                    t.buf[t.rd] = 0;
                    std::cout << t.buf;
                }
                if (t.rd == 0)
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
//    loop(tv, serv, t, cli, set);
    }
}
