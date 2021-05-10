//
// Created by Pamula File on 5/8/21.
//
#include "../header.h"

int		sorter(t_write a, t_write b)
{
	return (a.fd < b.fd);
}


static void parse_first_line(char *line, Header &head)
{
	char *tmp;

	tmp = strchr(line, ' ');
	head.setMethod(std::string(line, 0, tmp - line));
	line = strchr(tmp + 1, ' ') + 1;
	head.setRequest(std::string(tmp, 1, line - tmp - 2));
	tmp = strchr(line + 1, '\0');
	head.setHttp(std::string(line, 0, tmp - line - 1));

//	std::cout << "\n\n\nMethod =" << head.getMethod() << "\n\n\nRequest =" << head.getRequest() << "\n\n\nHttp =" << head.getHttp() << "\n\n\n" << std::endl;
}

static void parse_request(char *line, Header &head)
{
	char *tmp;
	
	if ((tmp = strstr(line, "Accept-Language: ")))
	{
		tmp += strlen("Accept-Language: ");
		head.setContent_Language("Content-Language: " + std::string(tmp));
	}
}

static void	communication_with_clients(std::list<t_write> &set, t_data &t, Header &head)
{
	char *line;
	int	 ct = 0;

	std::list<t_write>::iterator it = set.begin();
	while (it != set.end())
	{
		head.setEnv(t.env);
		if ( FD_ISSET((*it).fd, &t.read))
		{
			(*it).flag = 1;
			while ((t.rd = recive_next_line((*it).fd, &line)) > 0)
			{
				if (ct == 0)
				{
					parse_first_line(line, head);
					++ct;
				}
				else
					parse_request(line, head);

				std::cout << line << std::endl;
				free(line);
				line = 0;
			}
			if (t.rd == 0)
			{
				set.erase(it);
				break;
			}
			free(line);
			line = 0;
		}
		if ( FD_ISSET((*it).fd, &t.write))
		{
			int fd;
			send( (*it).fd, "HTTP/1.1 200 OK\n", 16, 0);
			std::string string;
			std::string string2;
			struct stat stat;
			char *str;
			str = (char *)head.getContent_Language().c_str();
			send( (*it).fd, str, strlen(str), 0);

			lstat("content/index.html", &stat);
			string = "Content-Length: ";
		   	string += std::to_string(stat.st_size + 1);
		  	str = (char *)string.c_str();
			send( (*it).fd, str, strlen(str), 0);
			fd = open("content/index.html", O_RDONLY);
			send((*it).fd, "\r\n\r\n", 4, 0);
			while (get_next_line(fd, &str))
			{
				send( (*it).fd, str, strlen(str), 0);
				send( (*it).fd, "\n", 1, 0);
				free(str);
				str = 0;
			}
			send( (*it).fd, str, strlen(str), 0);
			send( (*it).fd, "\n", 1, 0);
			free(str);
			str = 0;
			close(fd);
			head.eraseStruct();
		}
		it++;
	}
}

static int	Select(std::list<t_write> &set, t_data &t, timeval &tv, t_serv &serv)
{
	if (set.empty())
	{
		t.max_d = serv.host;
	}
	else
	{
		set.sort(sorter);
		t.max_d = (*set.rbegin()).fd > serv.host ? (*set.rbegin()).fd : serv.host;
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
    std::list<t_write>       set;
	std::list<t_write>::iterator it;
	Header head;

    (void)conf;
    while (true)
    {
//		std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
        init_fd_sets(t);
        FD_SET(serv.host, &t.read);
        it = set.begin();
        while ( it != set.end())
        {
            FD_SET((*it).fd, &t.read);
			if ((*it).flag)
           		 FD_SET((*it).fd, &t.write);             //if we have data to send
			(*it).flag = 0;
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
            t_write a = {cli.client, 0};
			set.push_back(a);
        }
		communication_with_clients(set, t, head);
//    loop(tv, serv, t, cli, set);
    }
}
