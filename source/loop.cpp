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
	head.addEnv((char *)("REQUEST_URI=" + head.getRequest()).c_str());
	head.addEnv((char *)("REQUEST_METHOD=" + head.getMethod()).c_str());
	head.addEnv((char *)("SERVER_PROTOCOL=" + head.getHttp()).c_str());
}

static void parse_request(char *line, Header &head)
{
	char *tmp;
	char *tmp2;
	std::string str;
	unsigned long int i = -1;
	int count = 0;

	if ((tmp = strstr(line, "Accept-Language: ")))
	{
		tmp += strlen("Accept-Language: ");
		head.setContent_Language("Content-Language: " + std::string(tmp) + "\n");
	}
	else if ((tmp = strstr(line, "Host: ")))
	{
		tmp += strlen("Host: ");
		tmp2 = strchr(tmp, ':');
		if (head.getHost().empty())
			head.setHost(std::string(tmp, 0, tmp2 - tmp));
		else
			(head.setHost("400"));
		head.setPort(std::stoi(tmp2 + 1));
	}
	else if ((tmp = strstr(line, "Referer: ")))
	{
		tmp += strlen("Referer: ");
		head.setReferer(tmp);
		head.addEnv((char *)("QUERY_STRING=" + head.getReferer()).c_str());
		while (line[++i])
		{
			if (line[i] == '/')
				count++;
			if (count == 3)
				break ;
		}
		str = std::string(line + i);
		if ((i = str.find('?')) != std::string::npos)
			str.erase(i);
		if ((i = str.rfind('.')) != std::string::npos)
			str.erase(0, i);
		if ((i = str.find('/')) != std::string::npos)
		{
			str.erase(0, i);
			head.addEnv((char *)("PATH_INFO=" + str).c_str());
		}
	}
}

std::string get_current_date()
{
	const static std::string daysOfWeek[] = {
		"Sun",
		"Mon",
		"Tue",
		"Wed",
		"Thu",
		"Fri",
		"Sat"
	};
	const static std::string monthsOfYear[] = {
		"January",
		"Fubruary",
		"March",
		"April",
		"May",
		"June",
		"August"
		"September"
		"October"
		"November"
		"December"
	};
  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];
  std::string date;
  std::string month;
  time (&rawtime);
  timeinfo = localtime(&rawtime);
  date = daysOfWeek[timeinfo->tm_wday] + ", ";
  strftime(buffer,sizeof(buffer),"%d",timeinfo);
  date += std::string(buffer) + " ";
  strftime(buffer,sizeof(buffer),"%m",timeinfo);
  date += monthsOfYear[atoi(buffer) - 1] + " ";
  strftime(buffer,sizeof(buffer),"%Y %H:%M:%S",timeinfo);
  date += std::string(buffer) + "\n";
  return date;
}

int recive(std::list<t_write> &set, std::list<t_write>::iterator &it, t_data &t)
{
	char *line;
	int ct = 0;

	if ( FD_ISSET((*it).fd, &t.read))
	{
		(*it).head.setEnv(t.env);
		(*it).flag = 1;
		while ((t.rd = recive_next_line((*it).fd, &line)) > 0)
		{
			if (ct == 0)
			{
				parse_first_line(line, (*it).head);
				++ct;
			}
			else
				parse_request(line, (*it).head);

		//	std::cout << line << std::endl;
			free(line);
			line = 0;
		}
		if (t.rd == 0)
		{
			set.erase(it);
			return 1;
		}
		free(line);
		line = 0;
	}
	return 0;
}

void response(std::list<t_write>::iterator &it, t_data &t, std::list<server> &conf)
{
	int fd;
	std::string string;
	std::string string2;
	struct stat stat;
	char *str;

	if ( FD_ISSET((*it).fd, &t.write))
	{
////////////////////////////////////
	  //  std::cout << "method: " << it->head.getMethod() << " request: " << it->head.getRequest() << " http: " << it->head.getHttp()  << "|" << std::endl;
	    if (it->head.getReferer().empty())
		{
			it->head.addEnv((char *)("QUERY_STRING=" + it->head.getRequest()).c_str());
		}
		fd = find_server(conf, (*it).head.getHost(), (*it).head.getPort()).responce((*it).head);
		fstat(fd, &stat);
		str = (char *)(*it).head.getHttp().c_str();
		send( (*it).fd, str, strlen(str), 0);
		str = (char *)(*it).head.getRequest().c_str();
		send( (*it).fd, str, strlen(str), 0);
		send( (*it).fd, " ", 1, 0);
		str = (char *)(*it).head.getMethod().c_str();
		send( (*it).fd, str, strlen(str), 0);
		send( (*it).fd, "\n", 1, 0);
		if (!((*it).head.getContent_Language().empty()))	
		{
			str = (char *)(*it).head.getContent_Language().c_str();
			send( (*it).fd, str, strlen(str), 0);
		}
		if (!((*it).head.getAllow().empty()))
		{
			str = (char *)(*it).head.getAllow().c_str();
			send( (*it).fd, str, strlen(str), 0);	
		}
		(*it).head.setDate("Date: " + get_current_date());
		str = (char *)(*it).head.getDate().c_str();
		send( (*it).fd, str, strlen(str), 0);
		str = (char *)(*it).head.getLast_Modified().c_str();
		send( (*it).fd, str, strlen(str), 0);
		string = "Content-Length: ";
		string += std::to_string(stat.st_size + 1);
		str = (char *)string.c_str();
		send( (*it).fd, str, strlen(str), 0);
		send((*it).fd, "\r\n\r\n", 4, 0);

///////////////////////////////////			
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
		(*it).head.eraseStruct();
	}

}

static void	communication_with_clients(std::list<t_write> &set, t_data &t, std::list<server> &conf)
{
	std::list<t_write>::iterator it = set.begin();
	while (it != set.end())
	{
		if (recive(set, it, t))
			break ;
		response(it, t, conf);
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
            t_write a = {Header(), cli.client, 0};
			set.push_back(a);
        }
		communication_with_clients(set, t, conf);
//    loop(tv, serv, t, cli, set);
    }
}
