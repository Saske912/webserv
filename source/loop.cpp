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
	int i = -1;
	int j;
	std::string str;

	while (line[++i])
		if (line[i] == ' ')
			break ;
    if (!line[i])
    {
        head.setMethod(std::string("THIS SHIT NOT IMPLEMENTED", 0, i));
        return ;
    }
	head.setMethod(std::string(line, 0, i));
	j = i + 1;
	while (line[++i])
		if (line[i] == ' ')
			break ;
	if (!line[i])
	{
        head.setMethod(std::string("THIS SHIT NOT IMPLEMENTED", 0, i));
        return ;
    }
	head.setRequest(std::string(line, j, i - j));
	j = i + 1;
	while (line[++i]);
	head.setHttp(std::string(line, j, i - j));
	head.addEnv((char *)("REQUEST_URI=" + head.getRequest()).c_str());
	head.addEnv((char *)("REQUEST_METHOD=" + head.getMethod()).c_str());
	head.addEnv((char *)("SERVER_PROTOCOL=" + head.getHttp()).c_str());
//	head.addEnv((char *)("PATH_INFO=" + head.getRequest()).c_str());
	str = head.getRequest();
	if (str.find('?') != std::string::npos)
		head.addEnv((char *)("QUERY_STRING=" + str.erase(0, str.find('?') + 1)).c_str());
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
        if (head.getHost().empty() and tmp2 != nullptr )
        {
            head.setHost(std::string(tmp, 0, tmp2 - tmp));
        }
		else
        {
            head.setHost("400");
        }
		head.setPort(atoi(tmp2 + 1));
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
    else if ((tmp = strstr(line, "Transfer-Encoding: ")))
    {

        tmp += strlen("Transfer-Encoding: ");
        head.setTransfer_Encoding(std::string(tmp));
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

int recv_next_line(int fd, char **line) {
    int z;
    char    buf[2];
    std::string str("");
    while (( z = recv( fd, buf, 1, 0 )) > 0 ) {
        if ( buf[0] == '\n' )
            break;
        buf[1] = 0;
        if ( buf[0] != '\r' )
            str += std::string( buf );
    }
    *line = strdup(str.c_str());
    return z;
}


int	chunked(std::list<t_write> &set, std::list<t_write>::iterator &it, t_data &t, std::list<server> &conf)
{
	char *line = 0;
	char *buf = 0;

	if (it->head.getFd() == 1)
		it->head.setFd(find_server(conf, (*it).head.getHost(), (*it).head.getPort()).responce((*it).head));
	if (it->count == 0)
		t.rd = recv_next_line((*it).fd, &line);
	if (t.rd == 0)
	{
		it->head.eraseStruct();
		it = set.erase(it);
		return 1;
	}
//			if (line)
//           	 std::cout << "line: " << line << std::endl;
	if (line && !line[0] && it->eshe_odin_ebychiy_flag) {
		if (it->head.getFd() != 1)
			close(it->head.getFd());
//                std::cout << "CHECK fILE" << std::endl;
		it->flag = true;
	}
	else {
		if (line && !line[0])
		{
			free(line);
			return 1;
		}
		if (line)
			 it->bytes = ( int ) strtol( line, 0, 16 );
		if (line && !it->bytes ) {
			it->eshe_odin_ebychiy_flag = true;
		} else {
			int n = it->bytes - it->count + 1 < 0? 0 : it->bytes - it->count;
//                    std::cout << "BUF FOR ALOCATE: " << it->bytes - it->count + 1  << std::endl;
			buf = (char *)malloc(sizeof(char) * (n + 1));
			t.rd = recv( it->fd, buf, n , 0 );
			if (t.rd == -1)
			{
				if (buf)
					free(buf);
				if (line)
					free(line);
                return 1;
			}
			if (it->head.getFd() != 1)
			{
				buf[t.rd] = 0;
				write(it->head.getFd(), buf, n);
				it->count += t.rd;
			}
			free(buf);
			if (it->count >= it->bytes)
				it->count = 0;
		}
	}
	if (line)
		free(line);
	if (t.rd == 0)
	{
		it->head.eraseStruct();
		it = set.erase(it);
		return 1;
	}
	return 0;
}

int recive(std::list<t_write> &set, std::list<t_write>::iterator &it, t_data &t, std::list<server> &conf)
{
	char    *line = 0;
	char    *buf;

	if ( FD_ISSET((*it).fd, &t.read))
	{
		if (!it->first_line)
		{
			(*it).head.setEnv(t.env);
			it->head.initEnv();
		}
        if ( it->head_readed && it->head.getTransfer_Encoding() == "chunked")
        {
			if (chunked(set, it, t, conf))
				return 1;
        }
        else
        {
            t.rd = recv_next_line((*it).fd, &line);
            if (t.rd != 0 and !line[0] and !it->first_line)
            {
                free(line);
                return 1;
            }
            if (!it->reminder.empty())
            {
                buf = line;
                line = ft_strjoin(it->reminder.c_str(), line);
                free(buf);
                buf = 0;
                it->reminder.erase();
            }
            if (t.rd == -1 && line[0])
            {
                it->reminder = std::string(line);
				free(line);
                return 1;
            }
            if (t.rd == 0)
            {
                it->head.eraseStruct();
                it = set.erase(it);
				if (line)
					free(line);
                return 1;
            }
            if (!line[0]) {
                it->head_readed = true;
            }
            else if ( !it->first_line )
            {
                it->first_line = true;
                parse_first_line(line, (*it).head);
            }
            else
                parse_request(line, (*it).head);
            if (line)
                std::cout << line  << std::endl;
            if (std::string(line).empty() && it->head.getTransfer_Encoding() != "chunked" \
					&& it->head.getMethod() != "PUT")
                (*it).flag = true;
        }
		if (line)
			free(line);
        line = 0;
	}
	return 0;
}

int sendFile(std::list<t_write>::iterator &it, int fd)
{
	char *str = 0;
    int stats = 0;
	
	while ((stats = get_next_line(fd, &str)))
	{
		if (stats == -1)
		{
			close(fd);
			return 1;
		}
		send( (*it).fd, str, strlen(str), 0);
		send( (*it).fd, "\n", 1, 0);
		free(str);
		str = 0;
	}
	send( (*it).fd, str, strlen(str), 0);
	send( (*it).fd, "\n", 1, 0);
	free(str);
	return 0;
}

void noBodyResponse(std::list<t_write>::iterator &it, int fd, std::list<t_write> &set)
{
	char *str = 0;

	str = (char *)(*it).head.getContent_Location().c_str();
	std::cout << str;
	send( (*it).fd, str, strlen(str), 0);
	if (fd != -1)
		close(fd);
	send((*it).fd, "\r\n", 2, 0);
	close(it->fd);
	it->head.eraseStruct();
	it = set.erase(it);
	std::cout << std::endl << "----------REQUEST----------" << std::endl;
}

void sendHeader(std::list<t_write>::iterator &it)
{
	char *str = 0;

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
}

void response(std::list<t_write>::iterator &it, t_data &t, std::list<server> &conf, std::list<t_write> &set)
{
	int fd;
	std::string string;
	std::string string2;
	struct stat stat;
	char *str;
	char *tmp;

	if ( FD_ISSET((*it).fd, &t.write))
	{
//		if (it->head.getFd() != 1)
//            close( it->head.getFd( ));
        it->first_line = false;
        it->head_readed = false;
		it->eshe_odin_ebychiy_flag = false;
		it->flag = false;
		it->bytes = 0;
		it->count = 0;
		it->head.addEnv((char *)"GATEWAY_INTERFACE=CGI/0.9");
		it->head.addEnv((char *)("REMOTE_ADDR=" + it->addr).c_str());
		it->head.addEnv((char *)"SERVER_SOFTWARE=webserv/1.0 (Unix)");
////////////////////////////////////
		fd = find_server(conf, (*it).head.getHost(), (*it).head.getPort()).responce((*it).head);
		std::cout << "----------RESPONSE----------" << std::endl;
		sendHeader(it);
				//|| (it->head.getMethod() == "POST" && it->head.getResponse() != "HTTP/1.1 405 Method Not Allowed\r\n")
		if (it->head.getMethod() == "PUT" || fd == -1)
			return noBodyResponse(it, fd, set);
        str = (char *)(*it).head.getContent_Location().c_str();
        std::cout << str;
        send( (*it).fd, str, strlen(str), 0);
		fstat(fd, &stat);
		string = "Content-Length: ";
		tmp = ft_itoa(stat.st_size + 1);
		string += std::string(tmp) + "\r\n";
		free(tmp);
		str = (char *)string.c_str();
		std::cout << str;
		send( (*it).fd, str, strlen(str), 0);
		send((*it).fd, "\r\n", 2, 0);

///////////////////////////////////
		if (sendFile(it, fd))
			return ;
		close(fd);
		(*it).head.eraseStruct();
		std::cout << std::endl << "----------REQUEST----------" << std::endl;
	}
}

static void	communication_with_clients(std::list<t_write> &set, t_data &t, std::list<server> &conf)
{
	std::list<t_write>::iterator it = set.begin();
	while (it != set.end())
	{
//	    std::cout << "rec"  << std::endl;
		if (recive(set, it, t, conf))
            continue ;
//        std::cout << "no break"  << std::endl;
		response(it, t, conf, set);
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
	char *tmp[4];
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
			tmp[0] = ft_itoa(cli.ad.sin_addr.s_addr & 255);
			tmp[1] = ft_itoa(cli.ad.sin_addr.s_addr >> 8 & 255);
			tmp[2] = ft_itoa(cli.ad.sin_addr.s_addr >> 16 & 255);
			tmp[3] = ft_itoa(cli.ad.sin_addr.s_addr >> 24);

            t_write a = {Header(), std::string(tmp[0]) + "." \
			+ std::string(tmp[1]) + "." + std::string(tmp[2]) + "." \
			+ std::string(tmp[3]), std::string(), cli.client, 0, 0, false, false, false, false};
			for (int i = 0 ; i < 4 ; i++)
				free(tmp[i]);
			set.push_back(a);
        }
		communication_with_clients(set, t, conf);
    }
}
