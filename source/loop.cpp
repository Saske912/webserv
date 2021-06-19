//
// Created by Pamula File on 5/8/21.
//
#include "../header.h"
#include "Number.hpp"

int		sorter(const Header& a, const Header& b)
{
	return (a.client < b.client);
}

std::string  receive_buffer(std::list<Header>::iterator &it)
{
    int         z;
    char        buf[BUFSIZE + 1];
    std::string ret;
    std::string buffer;

    z = recv( it->getFd(), buf, BUFSIZE, 0 );
    buffer = it->reminder + buf;
    it->reminder.erase();
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
        return std::string("connection closed");
    }
    else
    {
        return split_buffer(buffer, *it);
    }
}

int receive( std::list<Header>::iterator &it, config &conf)
{
	if ( FD_ISSET(it->client, &conf.read))
	{
        std::string str;

        if (!it->getFd())
            it->setFd(conf.find_server(it->getHost(), it->getPort()).responce(*it));
        str = receive_buffer( it );
        if (str == "connection closed")
            return 1;
        else if (str == "body_end")
        {
            return 0;
        }
	}
    return 0;
}


static void	communication_with_clients( std::list<Header> &set, config &conf)
{
    std::list<Header>::iterator it = set.begin();
    while (it != set.end())
    {
        if (file_available(it->getRequest()))
        {
            if (receive(it, conf))
            {
                it = set.erase(it);
                continue ;
            }
            response(it, conf);
        }
        if (it != set.end())
            it++;
    }
}

int sendFile(std::list<Header>::iterator &it, int fd, std::string &str2)
{
	char str[BUFSIZE + 1];
	int z;
    int ret;

	while ((z = read(fd, str, BUFSIZE)) > 0)
	{
		str[z] = 0;
		str2 += str;
	}
    if (send_protected(str2, it, "sendFile"))
        return 1;
	return 0;
}

//void noBodyResponse(std::list<Header>::iterator &it, int fd, std::list<Header> &set, std::string &str)
//{
//    if (send_protected(str + "\r\n", it, "noBodyResponse"))
//        return ;//send error
//    if (fd != -1)
//	    erase(it, fd, true);//
//    erase(it, it->client, true);//
//	it = set.erase(it);
//}

std::string  sendHeader(std::list<Header>::iterator &it)
{
	std::string str;

	str = it->getResponse();
    str += it->getContent_Language();
    str += it->getAllow();
	str += it->getDate();
	str += it->getLast_Modified();
	str += it->getContent_Location();
	return str;
}

int     parse_cgi(std::list<Header>::iterator &it, char *line)
{
	char *tmp;

	if ((tmp = strstr(line, "Status: ")))
	{
		tmp += strlen("Status: ");
		it->setResponse("HTTP/1.1 " + std::string(tmp) + "\n");
	}
	else if ((tmp = strstr(line, "Content-Type: ")))
		it->setContent_Type(std::string(tmp) + "\n");
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
//
//void resetIt(std::list<Header>::iterator &it)
//{
//	it->first_line = false;
//	it->head_readed = false;
//	it->eshe_odin_flag = false;
//	it->flag = false;
//	it->bytes = 0;
//	it->count = 0;
//	it->ct = 0;
//}

void  sendFileChunked( std::list<Header>::iterator &it, int fd)
{
	char line[BUFSIZE + 1];
	std::string str;
	int z;

    z = read(fd, line, BUFSIZE);
    std::cout << "z: " << z  << std::endl;
    if (z == 0)
    {
		if (waitpid(it->getPid(), 0, WNOHANG) == 0)
			return ;
        send_protected("0\r\n\r\n", it, "sendFileChunkedLast");
        erase( it );
        return ;
    }
    line[z] = 0;
    str = (getBaseSixteen(z) + "\r\n" + line + "\r\n");
    send_protected(str, it, "sendFileChunked");
}

void cgiResponse( std::list<Header>::iterator &it, int &fd)
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
	string = std::string(it->getResponse() + it->getContent_Type() + it->getDate() + it->getLast_Modified() + "Transfer-Encoding: chunked\r\n\r\n");
    if (send_protected(string, it, "sendFileChunked"))
        return ;
	sendFileChunked(it, fd);
}

//std::string  sendBodyHeader( std::list<Header>::iterator &it)
//{
//	std::string str;
//	struct stat stat;
//	int ret;
//
//	fstat(it->head.getFdr(), &stat);
//	return std::string("Content-Length: " + ttostr(stat.st_size) + "\r\n\r\n");
//}

void response( std::list<Header>::iterator &it, config &conf)
{
	std::string string;

	if (it->body_end)
	{
        sendHeader(it);
	}
}

static int	Select(config conf, std::list<Header> set)
{
	if (set.empty())
	{
        conf.max_d = conf.host;
	}
	else
	{
		set.sort(sorter);
        conf.max_d = (set.rbegin())->client > conf.host ? (set.rbegin())->client : conf.host;
		if ((conf.ret = select(conf.max_d + 1, &conf.read, NULL, NULL, &conf.tv)) < 1)
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
		if (!conf.ret)
		{
			return 1;
		}
	}
	return (0);
}

void    loop(config conf)
{
    std::list<Header>           set;
    std::list<Header>::iterator it;

    while (true)
    {
        FD_ZERO(&conf.read);
        FD_SET(conf.host, &conf.read);
        it = set.begin();
        while ( it != set.end())
        {
            FD_SET(it->client, &conf.read);
            it++;
        }
        if (Select(conf, set))
            continue ;
        if ( FD_ISSET(conf.host, &conf.read))
        {
            set.push_back(Header(conf));
        }
        communication_with_clients(set, conf);
    }
}
