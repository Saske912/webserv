//
// Created by Pamula File on 6/16/21.
//
#include "header.h"

int send_protected(std::string const & str, std::list<t_write>::iterator &it, std::string str2)
{
    int ret;

    ret = send(it->fd, str.c_str(), str.length(), 0);
    if (ret == -1)
    {
        perror("SEND ERROR");
        it->reminder = std::string(str);
        it->send_error = str2;
        return 1;
    }
    else if ((size_t)ret != str.length())
    {
        it->reminder = std::string(str, ret, str.length() - ret);
        it->send_error = str2;
        return 1;
    }
    return 0;
}
