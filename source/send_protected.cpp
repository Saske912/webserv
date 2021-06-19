//
// Created by Pamula File on 6/16/21.
//
#include "header.h"

int send_protected( std::string const & str, std::list<Header>::iterator &it, std::string str2)
{
    int ret;
    int len = str.length() < BUFSIZE ? str.length() : BUFSIZE;

//    std::cout << str  << std::endl;
    ret = send(it->getClient(), str.c_str(), len, 0);
    if (ret == -1)
    {
        perror("send");
        return 1;
    }
    else if ((size_t)ret != str.length())
    {
        std::cerr << "diff size (send)"  << std::endl;
//        it->reminder = std::string(str, ret, str.length() - ret);
//        it->send_error = str2;
        return 1;
    }
    return 0;
}