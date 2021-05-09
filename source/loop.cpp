//
// Created by Pamula File on 5/8/21.
//
#include "../header.h"


static void	communication_with_clients(std::set<int> &set, t_data &t)
{
		std::set<int>::iterator it = set.begin();
        while (it != set.end())
        {
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
}

static int	Select(std::set<int> &set, t_data &t, timeval &tv, t_serv &serv)
{
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
				return 1;//??
			}
//                loop(tv, serv, t, cli, set);
			return 1;
		}
		if (!t.ret)
		{
			//timeout
//                loop(tv, serv, t, cli, set);
			return 1;
		}
	}
	return (0);
}

void    loop(timeval &tv, t_serv &serv, t_data &t, std::list<server> &conf)
{
    t_client            cli;
    std::set<int>       set;
	std::set<int>::iterator it;
    (void)conf;
    while (true)
    {
        t = init_fd_sets();
        FD_SET(serv.host, &t.read);
        it = set.begin();
        while ( it != set.end())
        {
            FD_SET(*it, &t.read);
            FD_SET(*it, &t.write);             //if we have data to send
            it++;
        }
		if (Select(set, t, tv, serv))
			continue ;
        if ( FD_ISSET(serv.host, &t.read))
        {
            if (( cli.client = accept( serv.host, &cli.ad, &cli.adlen)) == -1)
                error_exit("fail to accept Client");
            fcntl( cli.client, F_SETFL, O_NONBLOCK);
            serv.opt = 1;
            setsockopt(cli.client, SOL_SOCKET, SO_NOSIGPIPE, &serv.opt, sizeof(serv.opt));
            set.insert(cli.client);
        }
		communication_with_clients(set, t);
//    loop(tv, serv, t, cli, set);
    }
}
