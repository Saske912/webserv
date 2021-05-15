//
// Created by Pamula File on 5/8/21.
//
#include "../header.h"

int		sorter(t_write a, t_write b)
{
	return (a.fd < b.fd);
}


static int parse_first_line(char *line, Header &head)
{
	int i = -1;
	int j;
	std::string str;

	while (line[++i])
		if (line[i] == ' ')
			break ;
	if (!line[i])
		return 1;
	head.setMethod(std::string(line, 0, i));
	j = i + 1;
	while (line[++i])
		if (line[i] == ' ')
			break ;
	if (!line[i])
		return 1;
	head.setRequest(std::string(line, j, i - j));
	j = i + 1;
	while (line[++i]);
	head.setHttp(std::string(line, j, i - j));
	head.addEnv((char *)("REQUEST_URI=" + head.getRequest()).c_str());
	head.addEnv((char *)("REQUEST_METHOD=" + head.getMethod()).c_str());
	head.addEnv((char *)("SERVER_PROTOCOL=" + head.getHttp()).c_str());
	str = head.getRequest();
	if (str.find('?') != std::string::npos)
		head.addEnv((char *)("QUERY_STRING=" + str.erase(0, str.find('?') + 1)).c_str());
	return 0;
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
			char tmpp[150];
			str.erase(0, i);
//			head.addEnv((char *)("PATH_INFO=" + str).c_str());
			head.addEnv((char *)("PATH_TRANSLATED=" + std::string(getcwd(tmpp, sizeof(tmpp))) + str).c_str());
		}
	}
	else if ((tmp = strstr(line, "Accept: ")))
	{
		tmp += strlen("Accept: ");
		str = std::string(tmp);
		head.addEnv((char *)("CONTENT_TYPE=" + std::string(str, 0, str.find(','))).c_str());
	}
	else if (head.getMethod() == "POST" && strchr(line, '='))
	{
		head.addEnv((char *)("QUERY_STRING=" + std::string(line)).c_str());
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
		std::cout << "----------REQUEST----------" << std::endl;
		(*it).head.setEnv(t.env);
		it->head.initEnv();
		(*it).flag = 1;
		while ((t.rd = recive_next_line((*it).fd, &line)) > 0)
		{
			if (ct == 0)
			{
				if (parse_first_line(line, (*it).head))
				    --ct;
				++ct;
			}
			else
				parse_request(line, (*it).head);

			std::cout << line << std::endl;
			free(line);
			line = 0;
		}
		if (t.rd == 0)
		{
			it->head.eraseStruct();
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

		it->head.addEnv((char *)"GATEWAY_INTERFACE=CGI/0.9");
		it->head.addEnv((char *)("REMOTE_ADDR=" + it->addr).c_str());
		it->head.addEnv((char *)"SERVER_SOFTWARE=webserv/1.0 (Unix)");
////////////////////////////////////
	  //  std::cout << "method: " << it->head.getMethod() << " request: " << it->head.getRequest() << " http: " << it->head.getHttp()  << "|" << std::endl;
		fd = find_server(conf, (*it).head.getHost(), (*it).head.getPort()).responce((*it).head);
		std::cout << "----------RESPONSE----------" << std::endl;
/*		str = (char *)(*it).head.getHttp().c_str();
		send( (*it).fd, str, strlen(str), 0);
		str = (char *)(*it).head.getRequest().c_str();
		send( (*it).fd, str, strlen(str), 0);
		send( (*it).fd, " ", 1, 0);
		str = (char *)(*it).head.getMethod().c_str();
		send( (*it).fd, str, strlen(str), 0);
		send( (*it).fd, "\n", 1, 0);*/
		str = (char *)(*it).head.getResponse().c_str();
		std::cout << str;
		send( (*it).fd, str, strlen(str), 0);
		if (!((*it).head.getContent_Language().empty()))	
		{
			str = (char *)(*it).head.getContent_Language().c_str();
		std::cout << str;
			send( (*it).fd, str, strlen(str), 0);
		}
		if (!((*it).head.getAllow().empty()))
		{
			str = (char *)(*it).head.getAllow().c_str();
		std::cout << str;
			send( (*it).fd, str, strlen(str), 0);	
		}
		(*it).head.setDate("Date: " + get_current_date());
		str = (char *)(*it).head.getDate().c_str();
		std::cout << str;
		send( (*it).fd, str, strlen(str), 0);
		str = (char *)(*it).head.getLast_Modified().c_str();
		std::cout << str;
		send( (*it).fd, str, strlen(str), 0);
		if (fd == -1)
			return ;
		fstat(fd, &stat);
		string = "Content-Length: ";
		string += std::to_string(stat.st_size + 1);
		str = (char *)string.c_str();
		std::cout << str;
		send( (*it).fd, str, strlen(str), 0);
		send((*it).fd, "\r\n\r\n", 4, 0);

///////////////////////////////////
        str = 0;
        int stat = 0;
		while ((stat = get_next_line(fd, &str)))
		{
		    if (stat == -1)
		    {
		        close(fd);
                return;
            }
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
            if (( cli.client = accept( serv.host, reinterpret_cast<sockaddr *>(&cli.ad), &cli.adlen)) == -1)
                error_exit("fail to accept Client");
            fcntl( cli.client, F_SETFL, O_NONBLOCK);
            serv.opt = 1;
            setsockopt(cli.client, SOL_SOCKET, SO_NOSIGPIPE, &serv.opt, sizeof(serv.opt));
            t_write a = {Header(), std::to_string(cli.ad.sin_addr.s_addr & 255) + "." + std::to_string(cli.ad.sin_addr.s_addr >> 8 & 255) + "." + std::to_string(cli.ad.sin_addr.s_addr >> 16 & 255) + "." + std::to_string(cli.ad.sin_addr.s_addr >> 24), cli.client, 0};
//			std::cout << a.addr << std::endl;
			set.push_back(a);
        }
		communication_with_clients(set, t, conf);
//    loop(tv, serv, t, cli, set);
    }
}
