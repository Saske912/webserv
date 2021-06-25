//
// Created by Pamula File on 5/8/21.
//
#include "../header.h"
#include "Number.hpp"

int		sorter(const Header& a, const Header& b)
{
	return (a.getClient() < b.getClient());
}

std::string receive_buffer( std::list<Header>::iterator &it, server &serv )
{
    int         z;
    char        buf[BUFSIZE + 1];
    std::string ret;
    std::string buffer;

    z = recv( it->getClient(), buf, BUFSIZE, 0 );
    buffer = it->getReminder() + buf;
    it->setReminder(std::string());
    if (z == -1)
    {
        if (errno == EAGAIN)
            return std::string("wait");
        else
            return std::string("error");
    }
    else if (z == 0)
        return std::string("connection closed");
    else
        return split_buffer( buffer, *it, serv );
}

int receive( std::list<Header>::iterator &it, server &serv )
{
	if ( FD_ISSET(it->getClient(), &serv.read))
	{
        std::string str;

        str = receive_buffer( it, serv );
        if (str == "connection closed")
            return 1;
        else if (str == "body_end")
            return 0;
	}
    return 0;
}


static void communication_with_clients( std::list<Header> &set, server &serv )
{
    int             opt;
    socklen_t len = sizeof(opt);

    std::list<Header>::iterator it = set.begin();
    while (it != set.end())
    {
        getsockopt( it->getClient( ), SOL_SOCKET, SO_ERROR, &opt, &len );
        if (opt == ECONNRESET)
        {
            std::cerr << "removed "  << std::endl;
            close(it->getClient());
            it = set.erase(it);
            continue ;
        }
        if (file_available(it->getRequest()))
        {
            if ( receive( it, serv ))
            {
                close(it->getClient());
                it = set.erase(it);
                continue ;
            }
            response( it, serv );
        }
        if (it != set.end())
            it++;
    }
}

void sendFile( Header &head )
{
	char    str[BUFSIZE + 1];
	size_t  z;

	while ((z = read(head.getFile(), str, BUFSIZE - head.getReminder().length())) > 0)
	{
		str[z] = 0;
        if ( send_protected(str, head))
            return ;
        bzero(str, sizeof(str));
	}
    erase(head.getRealPathToFile(), head);
}

void buildHeader( Header &head )
{
	std::string str;

	str = head.getResponse();
	str += head.getContent_Length();
    str += head.getContent_Language();
    str += head.getAllow();
	str += head.getDate();
	str += head.getLast_Modified();
	str += head.getContent_Location();
	str += head.getHostHeaderResponse();
	str += END;
	head.setEmptyLine(false);
	head.setReminder(str);
}

int     parse_cgi(std::list<Header>::iterator &it, char *line)
{
	char *tmp;

	if ((tmp = strstr(line, "Status: ")))
	{
		tmp += strlen("Status: ");
		it->setResponse("HTTP/1.1 " + std::string(tmp));
	}
	else if ((tmp = strstr(line, "Content-Type: ")))
		it->setContent_Type(std::string(tmp));
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

void clear( std::list<Header>::iterator &it)
{
    close(it->getClient());
    std::list<std::string >::iterator iter = Header::current_files_in_work.begin();
    while (iter != Header::current_files_in_work.end() and *iter != it->getRequest())
        iter++;
    if (iter != Header::current_files_in_work.end())
        Header::current_files_in_work.erase(iter);
    it->eraseStruct();
}

void  sendFileChunked( std::list<Header>::iterator &it, int fd)
{
	char line[BUFSIZE + 1];
	std::string str;
	int z;

    z = read(fd, line, BUFSIZE - 9 - it->getReminder().length());
    if (z == 0)
    {
//		if (waitpid(it->getPid(), 0, WNOHANG) == 0)
//			return ;
        send_protected("0\r\n\r\n", *it );
        clear(it);
        return ;
    }
    line[z] = 0;
    str = (getBaseSixteen(z) + "\r\n" + line + "\r\n");
    send_protected(str, *it );
}

//void cgiResponse( std::list<Header>::iterator &it, int &fd)
//{
//	char *line = 0;
//	int	 size = 0;
//	int     gnl;
//	int ret;
//	std::string string;
//
//	while ((gnl = get_next_line(fd, &line)) > 0)
//	{
//		if (line && line[0] == '\r')
//        {
//            size += 2;
//            free(line);
//            line = NULL;
//            break ;
//        }
//		ret = parse_cgi(it, line);
//		if (ret == 1)
//        {
//            lseek(fd, ((int)strlen(line) + 1) * (-1) ,SEEK_CUR);
//            break ;
//        }
//		size += strlen(line) + 1;
//		free(line);
//		line = NULL;
//	}
//	if (gnl == 0)
//    {
//	    std::cerr << "gnl = 0"  << std::endl;
//    }
//	else if (gnl == -1)
//    {
//	    std::cerr << "gnl error -1"  << std::endl;
//    }
//	string = std::string(it->getResponse() + it->getContent_Type() + it->getDate() + it->getLast_Modified() + "Transfer-Encoding: chunked\r\n\r\n");
//    if ( send_protected( string,  ))
//        return ;
//	sendFileChunked(it, fd);
//}

void response( std::list<Header>::iterator &it, server &serv )
{
	std::string string;
	if (it->isBodyEnd())
	{
        if (it->isEmptyLine())
            buildHeader( *it );
        if (it->getTransfer_Encoding() == "chunked" and !it->getError())
            sendFileChunked(it, it->getFile());
        else
            sendFile( *it );
	}
}

static int Select( std::list<Header> &set, server &serv )
{
    int ret;
	if (set.empty())
        serv.setMaxD( serv.getHostSock( ));
	else
    {
        set.sort(sorter);
        serv.setMaxD(set.begin()->getClient() > serv.getHostSock( ) ? set.begin()->getClient() : serv.getHostSock( ));
    }
    if ((ret = select(serv.getMaxD() + 1, &serv.read, NULL, NULL, &serv.tv)) < 1)
    {
        if (errno != EINTR)
        {
            perror("EINTR");
            return 1;
        }
        else
        {
            perror("errno");
            return 1;
        }
    }
    if (!ret)
    {
        return 1;
    }
	return (0);
}

void    loop(config &conf)
{
    std::list<Header>::iterator it;

    while (true)
    {
        std::list<server>::iterator it_serv = conf.getServers().begin();
        while (it_serv != conf.getServers().end())
        {
            FD_ZERO(&it_serv->read);
            FD_SET( it_serv->getHostSock( ), &it_serv->read);
            it = it_serv->getSet().begin();
            while ( it != it_serv->getSet().end())
            {
                FD_SET(it->getClient(), &it_serv->read);
                it++;
            }
            if ( Select( it_serv->getSet( ), *it_serv ))
                continue ;
            if (FD_ISSET( it_serv->getHostSock( ), &it_serv->read))
            {
                it_serv->getSet().push_back( Header( *it_serv, NULL ));
            }
            communication_with_clients( it_serv->getSet( ), *it_serv );
        }
    }
}
