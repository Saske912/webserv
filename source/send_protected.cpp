//
// Created by Pamula File on 6/16/21.
//
#include "header.h"

int send_protected( std::string const &str, Header &head )
{
    ssize_t  ret;

    ret = send(head.getClient(), str.c_str(), BUFSIZE, 0);
    if (ret == -1)
    {
        head.setReminder(str);
        perror("send");
        return 1;
    }
    else if ((size_t)ret != str.length())
    {
        std::cerr << "diff size (send)"  << std::endl;
        head.setReminder(std::string(str, ret, str.length() - ret));
        return 1;
    }
    head.setReminder(std::string());
    return 0;
}