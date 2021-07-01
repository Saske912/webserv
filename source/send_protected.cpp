//
// Created by Pamula File on 6/16/21.
//
#include "header.h"

int send_protected( std::string str, Header &head )
{
    ssize_t  ret;

	str = head.getReminder() + str;
	if (str.length() > BUFSIZE)
    {
	    ret = send(head.getClient(), str.c_str(), BUFSIZE, 0);
    }
	else
        ret = send(head.getClient(), str.c_str(), str.length(), 0);
    if (ret == -1)
    {
        head.setReminder(str);
        perror("send");
        return 1;
    }
    else if ((size_t)ret != str.length())
    {
        head.setReminder(std::string(str, ret, str.length() - ret));
        std::cout << "diff size (send)" << head.getReminder().length() << std::endl;
        return 1;
    }
//    std::cout << str  << std::endl;
    head.setReminder(std::string());
    return 0;
}
