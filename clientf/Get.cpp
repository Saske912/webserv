//#include "../header.h"
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>
#define CLIENTS 100
#define REQUESTS 10000
#define IP "10.21.34.44"
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;

typedef struct s_str
{
    int     sock;
    int     err;
}           t_str;

void    *func(void *t)
{
    int     sock;
    std::string tmp = "GET / HTTP/1.1\r\nHost: 10.21.34.44:1025\r\n\r\n";
    t_str  *st = (t_str *)t;
    char buf[32769];
    int ret;
    int cnt = REQUESTS;
    struct sockaddr_in  addr;
    socklen_t           addrlen;
    std::string     ip = IP;
    int num;
    bool flag = false;

    pthread_mutex_lock(&mutex2);
    num = st->sock;
    st->sock++;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1024);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addrlen = sizeof(addr);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(sock, reinterpret_cast<sockaddr *>(&addr), addrlen) == -1)
        exit(1);
    pthread_mutex_unlock(&mutex2);
    while ( cnt-- )
    {
        if (!flag)
        {
            pthread_mutex_lock(&mutex);
            if (send(sock, tmp.c_str(), tmp.length(), 0) == -1)
            {
                perror("send");
                exit(1);
            }
			std::cout << num << ": request(" << REQUESTS - cnt << ") sended" << std::endl;
            flag = true;
            pthread_mutex_unlock(&mutex);
        }
        if ( flag )
        {
            ret = recv(sock, buf, 32768, 0);
//               std::cout << buf  << std::endl;
            pthread_mutex_lock(&mutex3);
            if (ret < 0)
            {
                perror("recive");
                exit(1);
            }
            buf[ret] = 0;
            if (ret == -1)
            {
//                std::cout << "ret: " << ret << " buf: " << buf  << std::endl;
                st->err = 1;
            }
            std::cout << num << ": response recieved" << std::endl;
            pthread_mutex_unlock(&mutex3);
            flag = false;
        }
    }
    std::cout << num << ": finished" << std::endl;

    return NULL;
}

int main( )
{
    t_str st;
    st.err = 0;
    st.sock = 0;

    int cnt = CLIENTS;
    pthread_t   tred[CLIENTS];
    while(cnt--)
    {
        if (pthread_create(&tred[cnt], NULL, func, &st))
        {
            perror("pthread_c");
            exit(1);
        }
     //   std::cout << "cnt: " << cnt  << std::endl;

//        sleep(1);
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
