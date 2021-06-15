//
// Created by Pamula File on 5/8/21.
//
#include "../header.h"
#include "Number.hpp"

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
	head.addEnv((char *)("PATH_INFO=" + head.getRequest()).c_str());
	head.addEnv((char *)("PATH_TRANSLATED=" + head.getRequest()).c_str());
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
        if (head.getHost().empty() and tmp2 != NULL )
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
			head.addEnv((char *)("PATH_INFO=" + str).c_str());
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
    else if ((tmp = strstr(line, "Authorization: ")))
    {
        tmp += strlen("Authorization: ");
		head.addEnv((char *)("AUTH_TYPE=" + std::string(tmp, 0, strchr(tmp, ' ') - tmp)).c_str());
		if (std::string(head.getEnvValue("AUTH_TYPE=")) == "BASIC" || std::string(head.getEnvValue("AUTH_TYPE=")) == "DIGEST")
		{
			tmp = strchr(tmp, ' ') + 1;
			head.addEnv((char *)("REMOTE_USER=" + std::string(tmp, 0, strchr(tmp, ':') - tmp)).c_str());
			tmp = strchr(tmp, ':') + 1;
			head.addEnv((char *)("REMOTE_IDENT=" + std::string(tmp)).c_str());
		}
    }
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
	struct stat stat;
	if (it->head.getFd() == 1)
    {
        it->head.setFd(find_server(conf, (*it).head.getHost(), (*it).head.getPort()).responce((*it).head));
        if (it->head.getFd() == -2)
        {
            it->head.setFd(1);
            return 1;
        }
    }
	if (it->count == 0)
	{
		t.rd = recv_next_line((*it).fd, &line);
		if (t.rd == -1 && line && line[0])
		{
			if (it->reminder.empty())
				it->reminder = std::string(line);
			else
				it->reminder += std::string(line);
			free(line);
			return 1;
		}
        if (!it->reminder.empty())
		{
			if (line)
			{
				buf = line;
				line = strdup((it->reminder + std::string(line)).c_str());
				free(buf);
				buf = 0;
			}
			else
				line = strdup(it->reminder.c_str());
			it->reminder.erase();
		}
        if (!t.rd)
        {
            erase(it, it->fd, false);
		    it = set.erase(it);
            return 1;
        }
		if (line && std::string(line).find_last_not_of("1234567890abcdef") != std::string::npos)
		{
			free(line);
			return 1;
		}
	}
    if (line && !line[0] && it->eshe_odin_flag) {
		if (it->head.getFd() != 1)
		{
			fstat(it->head.getFd(), &stat);
			it->head.addEnv(("CONTENT_LENGTH=" + ttostr(stat.st_size + 1)).c_str());
			erase(it, it->head.getFd(), false);
			it->ct = 0;
		}
		it->flag = true;
	}
	else
	{
        if (line && !line[0])
		{
			free(line);
			return 1;
		}
		if (line)
		{
			 it->bytes = ( int ) strtol( line, 0, 16 );
		}
		if (line && !it->bytes)
		{
			it->eshe_odin_flag = true;
			it->head.setBodySize(it->ct);
		}
		else
		{
			int n = it->bytes - it->count;
			buf = (char *)malloc(sizeof(char) * (n + 1));
			t.rd = recv( it->fd, buf, n , 0 );
            if (t.rd == 0)
            {
                erase(it, it->fd, false);
                it = set.erase(it);
                return 1;
            }
			if (t.rd == -1 && !buf[0])
			{
				free(buf);
				return 1;
			}
			if (t.rd == -1)
			{
				t.rd = strlen(buf);
			}
			if (it->head.getFd() != 1)
			{
			    if (t.rd != -1)
                {
                    buf[t.rd] = 0;
                    write(it->head.getFd(), buf, t.rd);
                }
			}
			if (t.rd != -1)
			  it->ct += t.rd;
			it->count += t.rd;
			free(buf);
			if (it->count >= it->bytes)
			{
				it->count = 0;
			}
		}
	}
	if (line)
		free(line);
	return 0;
}

void recive( std::list<t_write> &set, std::list<t_write>::iterator &it, t_data &t, std::list<server> &conf)
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
				return ;
        }
        else
        {
            t.rd = recv_next_line((*it).fd, &line);
            if (t.rd != 0 and !line[0] and !it->first_line)
            {
                free(line);
                return ;
            }
            if (!it->reminder.empty())
            {
                buf = line;
                line = strdup((it->reminder + line).c_str());
                free(buf);
                buf = 0;
                it->reminder.erase();
            }
            if (t.rd == -1 && line[0])
            {
                it->reminder = std::string(line);
				free(line);
                return ;
            }
            if (t.rd == 0)
            {
                erase(it, it->fd, false);
                it = set.erase(it);
                free(line);
                return ;
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
//                std::cout << line  << std::endl;
            if (std::string(line).empty() && it->head.getTransfer_Encoding() != "chunked" \
                    && it->head.getMethod() != "PUT")
                (*it).flag = true;
        }
        free(line);
        line = 0;
	}
}

int sendFile(std::list<t_write>::iterator &it, int fd)
{
	char str[32769];
	int z;
    int ret;

	while ((z = read(fd, str, 32768)) > 0)
	{
		str[z] = 0;
		ret = send(it->fd, str, z, 0);
		if (ret == -1)
		{
			it->reminder = std::string(str);
			it->send_error = "sendFile";
			return 1;
		}
		else if ((size_t)ret != strlen(str))
        {
        	it->reminder = std::string(str, ret, strlen(str) - ret);
			it->send_error = "sendFile";
			return 1;
		}
	}
	return 0;
}

void noBodyResponse(std::list<t_write>::iterator &it, int fd, std::list<t_write> &set)
{
	char *str = 0;
	int ret;
    std::list<std::string >::iterator iter;

	std::cout << "kek1"  << std::endl;
	if (fd != -1)
	    erase(it, fd, false);
	str = (char *)((*it).head.getContent_Location() + "\r\n").c_str();
	ret = send( (*it).fd, str, strlen(str), 0);
    if (ret == -1)
	{
		it->reminder = std::string(str);
		it->send_error = "noBodyResponse";
		return ;
	}
	else if ((size_t)ret != strlen(str))
	{
		it->reminder = std::string(str, ret, strlen(str) - ret);
		it->send_error = "noBodyResponse";
		return ;
	}
    erase(it, it->fd, false);
	it = set.erase(it);
}

int sendHeader(std::list<t_write>::iterator &it)
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
//	std::cout << str;
	ret = send(it->fd, str.c_str(), str.length(), 0);
	if (ret == -1)
	{
		it->reminder = std::string(str);
		it->send_error = "sendHeader";
		return 1;
	}
	else if ((size_t)ret != str.length())
	{
		it->reminder = std::string(str, ret, str.length() - ret);
		it->send_error = "sendHeader";
		return 1;
	}
	return 0;
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
	char line[32769];
	char *str;
	int z;

    z = read(fd, line, 1000);
    if (z == 0)
    {
		if (waitpid(it->head.getPid(), 0, WNOHANG) == 0)
			return ;
        z = send(it->fd, "0\r\n\r\n", 5, 0);
		if (z == -1)
		{
			it->send_error = "sendFileChunkedLast";
			it->reminder = std::string("0\r\n\r\n");
			return ;
		}
		else if ((size_t)z != 5)
		{
			it->send_error = "sendFileChunkedLast";
			it->reminder = std::string("0\r\n\r\n", z, 5 - z);
			return ;
		}
        erase(it, fd, true);
        return ;
    }
    line[z] = 0;
    str = strdup((getBaseSixteen(z) + "\r\n" + line + "\r\n").c_str());
    z = send(it->fd, str, strlen(str), 0);
	if (z == -1)
	{
		it->send_error = "sendFileChunked";
		it->reminder = std::string(str);
	}
	else if ((size_t)z != strlen(str))
	{
		it->send_error = "sendFileChunked";
		it->reminder = std::string(str, z, strlen(str) - z);
	}
	free(str);
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
	ret = send(it->fd, string.c_str(), string.length(), 0);
	if (ret == -1)
	{
		it->reminder = std::string(string);
		it->send_error = "sendFileChunked";
		return ;
	}
	else if ((size_t)ret != string.length())
	{
		it->reminder = std::string(string, ret, string.length() - ret);
		it->send_error = "sendFileChunked";
		return ;
	}
	sendFileChunked(it, fd);
}

int sendBodyHeader(std::list<t_write>::iterator &it)
{
	std::string str;
	struct stat stat;
	int ret;

	fstat(it->head.getFdr(), &stat);
	str = std::string((*it).head.getContent_Location() + "Content-Length: " + ttostr(stat.st_size) + "\r\n\r\n");
    ret = send( (*it).fd, str.c_str(), str.length(), 0);
	if (ret == -1)
	{
		it->reminder = std::string(str);
		it->send_error = "sendBodyHeader";
		return 1;
	}
	else if ((size_t)ret != str.length())
	{
		it->reminder = std::string(str, ret, str.length() - ret);
		it->send_error = "sendBodyHeader";
		return 1;
	}
	return 0;
}

void response(std::list<t_write>::iterator &it, t_data &t, std::list<server> &conf, std::list<t_write> &set)
{
	int fd;
	std::string string;
	std::string string2;
	char *str;
	int z;

	if ( FD_ISSET((*it).fd, &t.write))
	{
		if (!it->reminder.empty())
		{
			str = strdup(it->reminder.c_str());
			z = send(it->fd, str, strlen(str), 0);
			if (z == -1)
			{
				it->reminder = std::string(str);
				free(str);
				return ;
			}
			else if ((size_t)z != strlen(str))
			{
				it->reminder = std::string(str, z, strlen(str) - z);
				free(str);
				return ;
			}
			free(str);
			str = 0;
			it->reminder.erase();
			if (it->send_error == "sendFile")
			{
				it->send_error.erase();
				goto sendFile;
			}
			else if (it->send_error == "sendHeader")
			{
				it->send_error.erase();
				goto sendHeader;
			}
			else if (it->send_error == "sendBodyHeader")
			{
				it->send_error.erase();
				goto sendBodyHeader;
			}
			else if (it->send_error == "sendFileChunkedLast")
			{
				it->send_error.erase();
				close(it->head.getFdr());
                std::list<std::string>::iterator iter = Header::current_files_in_work.begin();
                while (iter != Header::current_files_in_work.end() and *iter != it->head.getRequest())
                    iter++;
                if (iter != Header::current_files_in_work.end())
                    Header::current_files_in_work.erase(iter);
				it->head.eraseStruct();
				resetIt(it);
				return ;
			}
			else if (it->send_error == "noBodyResponse")
			{
				it->send_error.erase();
                erase(it, it->fd, false);
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
		if (sendHeader(it))
			return ;
sendHeader:
		if (it->head.getMethod() == "PUT" || it->head.getFdr() == -1)
			return noBodyResponse(it, it->head.getFdr(), set);
   
		if (sendBodyHeader(it))
			return ;
sendBodyHeader:
sendFile:
		if (sendFile(it, it->head.getFdr()))
			return ;
		erase(it, it->head.getFdr(), true);
	}
}

static void	communication_with_clients(std::list<t_write> &set, t_data &t, std::list<server> &conf)
{
    int i = 0;
	std::list<t_write>::iterator it = set.begin();
	while (it != set.end())
	{
		recive(set, it, t, conf);
        if (it == set.end())
            continue;
		response(it, t, conf, set);
		if (it != set.end())
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
