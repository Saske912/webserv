//#include "../header.h"
#include <sys/socket.h>
#include <sys/select.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#define IP "10.21.31.85"
#define CLIENTS 100
#define REQUESTS 10000
#define HEADER "POST /directory/youpi.bla HTTP/1.1\r\nHost: 10.21.31.85:1025\r\nTransfer-Encoding: chunked\r\n\r\n"
#define FILE_TO_OPEN "youpi.bla"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;

typedef struct s_str
{
    int     sock;
    int     err;
}           t_str;

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

void    *func(void *t)
{
    int     sock;
    std::string tmp = HEADER;
    t_str  *st = (t_str *)t;
	std::string str;
    char buf[32769];
    int ret;
    int cnt = REQUESTS;
    struct sockaddr_in  addr;
    socklen_t           addrlen;
    std::string     ip = IP;
    int num;
	int fd;
	int file_sended = false;

    pthread_mutex_lock(&mutex2);
    num = st->sock;
    st->sock++;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1025);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addrlen = sizeof(addr);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(sock, reinterpret_cast<sockaddr *>(&addr), addrlen) == -1)
        exit(1);
    pthread_mutex_unlock(&mutex2);

    while ( true )
    {
        if (file_sended)
        {
            ret = recv(sock, buf, 32768, 0);
         //   pthread_mutex_lock(&mutex3);
            if (ret < 0)
            {
                perror("recive");
                exit(1);
            }
            buf[ret] = 0;
//			std::cout << buf << std::endl;
			if (strstr(buf, "\r\n\r\n"))
			{
				std::cout << num << ": file recieved" << std::endl;
				file_sended = false;
			}
            if (ret == -1)
                st->err = 1;
           // pthread_mutex_unlock(&mutex3);
        }
		if (!file_sended)
        {
			if (cnt == 0)
				break ;
			--cnt;
            pthread_mutex_lock(&mutex);
            if (send(sock, tmp.c_str(), tmp.length(), 0) == -1)
            {
                perror("send");
                exit(1);
            }
			fd = open(FILE_TO_OPEN, O_RDONLY);
			while ((ret = read(fd, buf, rand() % 32768)) > 0)
			{
				buf[ret] = 0;
				str = getBaseSixteen(ret) + "\r\n" + buf + "\r\n";
				send(sock, str.c_str(), str.length(), 0);
	//			std::cout << getBaseSixteen(ret) << std::endl;
				bzero(buf, 32768);
			}
			bzero(buf, sizeof(buf));
			send(sock, "0\r\n\r\n", 5, 0);
			std::cout << num << ": file(" << REQUESTS - cnt << ") sended" << std::endl;
			close(fd);
            pthread_mutex_unlock(&mutex);
			file_sended = true;
        }
    }
	std::cout << num << ": finished" << std::endl;
	close(sock);
    return NULL;
}

int main( )
{
    t_str st;
    st.err = 0;
    st.sock = 0;

    int cnt = CLIENTS;
    pthread_t   tred[CLIENTS];
	std::srand(std::time(0));
    while(cnt--)
    {
        if (pthread_create(&tred[cnt], NULL, func, &st))
        {
            perror("pthread_c");
            exit(1);
        }
    }
    cnt = CLIENTS;
    while (cnt--)
    {
        pthread_join(tred[cnt], NULL);
    }
    if (st.err)
    {
        std::cout << "err finded"  << std::endl;
        perror("error");
    }
//    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}
