//
// Created by Pamula File on 5/8/21.
//
#include "../header.h"
#include "Number.hpp"

int		sorter(t_write a, t_write b)
{
	return (a.fd < b.fd);
}

std::string  receive_buffer( int fd, std::list<t_write> &set, std::list<t_write>::iterator &it)
{
    int         z;
    char        buf[BUFSIZE + 1];
    std::string ret;
    std::string buffer;

    z = recv( fd, buf, BUFSIZE, 0 );
    buffer = it->head.reminder + buf;
    it->head.reminder.erase();
    if (z == -1)
    {
        if (errno == EAGAIN)
        {
            return std::string("wait");
        }
        else
        {
            perror("errno");
            return std::string("error");
        }
    }
    else if (z == 0)
    {
        it = set.erase(it);
        return std::string("connection closed");
    }
    else
    {
        return split_buffer(buffer, it->head);
    }
}

int receive( std::list<t_write> &set, std::list<t_write>::iterator &it, t_data &t, std::list<server> &conf)
{
	if ( FD_ISSET(it->fd, &t.read))
	{
        std::string str;

        it->head.setFd(find_server(conf, it->head.getHost(), it->head.getPort()).responce(it->head));
        str = receive_buffer( it->fd, set, it );
        if (str == "connection closed")
            return 1;
	}
}


static void	communication_with_clients(std::list<t_write> &set, t_data &t, std::list<server> &conf)
{
    int ret;

    std::list<t_write>::iterator it = set.begin();
    while (it != set.end())
    {
        if (file_available(it->head.getRequest()))
        {
            ret = receive(set, it, t, conf);
            if (ret == 1)
                continue ;
            response(it, t, conf, set);
        }
        if (it != set.end())
            it++;
    }
}

int sendFile(std::list<t_write>::iterator &it, int fd, std::string &str2)
{
	char str[BUFSIZE + 1];
	int z;
    int ret;

//    std::cout << "fd: " << it->fd  << std::endl;
	while ((z = read(fd, str, BUFSIZE)) > 0)
	{
		str[z] = 0;
		str2 += str;
	}
    if (send_protected(str2, it, "sendFile"))
        return 1;
	return 0;
}

void noBodyResponse(std::list<t_write>::iterator &it, int fd, std::list<t_write> &set, std::string &str)
{
    if (send_protected(str + "\r\n", it, "noBodyResponse"))
        return ;//send error
    if (fd != -1)
	    erase(it, fd, true);//
    erase(it, it->fd, true);//
	it = set.erase(it);
//    std::cout << "NoBody"  << std::endl;
//all right
}

std::string  sendHeader(std::list<t_write>::iterator &it)
{
	std::string str;
	int    ret;

	str = (*it).head.getResponse();
	if (!((*it).head.getContent_Language().empty()))	
	{
		str += (*it).head.getContent_Language();
	}
	if (!((*it).head.getAllow().empty()))
	{
		str += (*it).head.getAllow();
	}
	(*it).head.setDate("Date: " + get_current_date());
	str += (*it).head.getDate();
	str += (*it).head.getLast_Modified();
	str += (*it).head.getContent_Location();
	return str;
}

int     parse_cgi(std::list<t_write>::iterator &it, char *line)
{
	char *tmp;

	if ((tmp = strstr(line, "Status: ")))
	{
		tmp += strlen("Status: ");
		it->head.setResponse("HTTP/1.1 " + std::string(tmp) + "\n");
	}
	else if ((tmp = strstr(line, "Content-Type: ")))
		it->head.setContent_Type(std::string(tmp) + "\n");
	else
        return 1;
    return 0;
}

std::string getBaseSixteen(unsigned int n)
{
	std::string str("");
	unsigned int num = n;
	int c = 1;

	while (num / 16)
	{
		num /= 16;
		c *= 16;
	}
	while (c != 0)
	{
		str += n / c < 10 ? n / c + 48 : n / c + 22 + 'A';
		n %= c;
		c /= 16;
	}
	return str;
}

void resetIt(std::list<t_write>::iterator &it)
{
	it->first_line = false;
	it->head_readed = false;
	it->eshe_odin_flag = false;
	it->flag = false;
	it->bytes = 0;
	it->count = 0;
	it->ct = 0;
}

void  sendFileChunked(std::list<t_write>::iterator &it, int fd)
{
	char line[BUFSIZE + 1];
	std::string str;
	int z;

    z = read(fd, line, BUFSIZE);
    std::cout << "z: " << z  << std::endl;
    if (z == 0)
    {
		if (waitpid(it->head.getPid(), 0, WNOHANG) == 0)
			return ;
        send_protected("0\r\n\r\n", it, "sendFileChunkedLast");
        erase(it, fd, true);
        return ;
    }
    line[z] = 0;
    str = (getBaseSixteen(z) + "\r\n" + line + "\r\n");
    send_protected(str, it, "sendFileChunked");
}

void cgiResponse(std::list<t_write>::iterator &it, int &fd)
{
	char *line = 0;
	int	 size = 0;
	int     gnl;
	int ret;
	std::string string;

	while ((gnl = get_next_line(fd, &line)) > 0)
	{
		if (line && line[0] == '\r')
        {
            size += 2;
            free(line);
            line = NULL;
            break ;
        }
		ret = parse_cgi(it, line);
		if (ret == 1)
        {
            lseek(fd, ((int)strlen(line) + 1) * (-1) ,SEEK_CUR);
            break ;
        }
		size += strlen(line) + 1;
		free(line);
		line = NULL;
	}
	if (gnl == 0)
    {
	    std::cerr << "gnl = 0"  << std::endl;
    }
	else if (gnl == -1)
    {
	    std::cerr << "gnl error -1"  << std::endl;
    }
	if (it->head.getResponse().empty())
	    it->head.setResponse("HTTP/1.1 200 OK\r\n");
	string = std::string(it->head.getResponse() + it->head.getContent_Type() + (*it).head.getDate() + (*it).head.getLast_Modified() + "Transfer-Encoding: chunked\r\n\r\n");
    if (send_protected(string, it, "sendFileChunked"))
        return ;
	sendFileChunked(it, fd);
}

std::string  sendBodyHeader(std::list<t_write>::iterator &it)
{
	std::string str;
	struct stat stat;
	int ret;

	fstat(it->head.getFdr(), &stat);
	return std::string("Content-Length: " + ttostr(stat.st_size) + "\r\n\r\n");
}

void response(std::list<t_write>::iterator &it, t_data &t, std::list<server> &conf, std::list<t_write> &set)
{
	int fd;
	std::string string;
	std::string string2;
	char *str;
	int z;

	if ( FD_ISSET((*it).fd, &t.write))
//	if (it->flag)
	{
		if (!it->reminder.empty())
		{
            if (send_protected(it->reminder, it, it->send_error))
                return ;
			it->reminder.erase();
			if (it->send_error == "sendFile" || it->send_error == "sendFileChunkedLast")
			{
				it->send_error.erase();
                erase(it, it->head.getFdr(), true);
                return;
			}
			else if (it->send_error == "noBodyResponse")
			{
				it->send_error.erase();
                erase(it, it->fd, true);//
				it = set.erase(it);
				return ;
			}
		}
		if (it->head.getFdr())
			return (sendFileChunked(it, it->head.getFdr()));
		it->head.addEnv((char *)"GATEWAY_INTERFACE=CGI/0.9");
		it->head.addEnv((char *)("REMOTE_ADDR=" + it->addr).c_str());
		it->head.addEnv((char *)"SERVER_SOFTWARE=webserv/1.0 (Unix)");
		it->head.setFdr(find_server(conf, (*it).head.getHost(), (*it).head.getPort()).responce((*it).head));
		fd = it->head.getFdr();
		if (fd == -2)
        {
		    it->head.setFdr(0);
            return;
        }
		chdir(it->head.getEnvValue("PWD=").c_str());
		if (it->head.getIsCgi()) {
			return cgiResponse(it, it->head.getFdr());
		}
		string2 = sendHeader(it);
		if (it->head.getMethod() == "PUT" || it->head.getFdr() == -1)
			return (noBodyResponse(it, it->head.getFdr(), set, string2));
		string2 += sendBodyHeader(it);
//        if (send_protected(string2, it, "sendFile"))
//            return ;
		if (sendFile(it, it->head.getFdr(), string2))
			return ;
		erase(it, it->head.getFdr(), true);
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
                return 1;
			else
			{
				return 1;//??
			}
//                loop(tv, serv, t, cli, set);
			return 1;
		}
		if (!t.ret)
		{
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
    std::string ipstr;
    while (true)
    {
        init_fd_sets(t);
        FD_SET(serv.host, &t.read);
        it = set.begin();
        while ( it != set.end())
        {
            if (!it->head.getFdr())
                FD_SET((*it).fd, &t.read);
            if ((*it).flag)
            {
                FD_SET(( *it ).fd, &t.write );
            }
            it++;
        }
        if (Select(set, t, tv, serv))
            continue ;
        if ( FD_ISSET(serv.host, &t.read))
        {
            if (( cli.client = accept( serv.host, reinterpret_cast<sockaddr *>(&cli.ad), &cli.adlen)) == -1)
                continue ;
            fcntl( cli.client, F_SETFL, O_NONBLOCK);
            serv.opt = 1;
            setsockopt(cli.client, SOL_SOCKET, SO_NOSIGPIPE, &serv.opt, sizeof(serv.opt));
            int bufer = BUFSIZE;
            setsockopt(cli.client, SOL_SOCKET, SO_SNDBUF, &bufer, sizeof(bufer));
            ipstr = ttostr(cli.ad.sin_addr.s_addr & 255) + '.' +
                    ttostr(cli.ad.sin_addr.s_addr >> 8 & 255) + '.' +
                    ttostr(cli.ad.sin_addr.s_addr >> 16 & 255) + '.' +
                    ttostr(cli.ad.sin_addr.s_addr >> 24);

            t_write a = {Header(), ipstr, std::string(), cli.client, 0, 0, 0, false, false, false, false, std::string()};
            set.push_back(a);
        }
        communication_with_clients(set, t, conf);
    }
}
