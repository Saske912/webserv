//
// Created by Pamula File on 6/17/21.
//
#include "header.h"

std::string  parse_request(const std::string& string, Header &head)
{
    if (head.empty_line)
    {
        if (head.getTransfer_Encoding() == "chunked")
        {
            if (string.find_last_not_of("0123456789abcdef") != std::string::npos)
            {
                if (write(head.getFd(), string.c_str(), string.length()) != static_cast<ssize_t>(string.length()))
                {
                    perror("write");
                    return "bad";
                }
            }
        }
        else
        {
            if (write(head.getFd(), string.c_str(), string.length()) != static_cast<ssize_t>(string.length()))
            {
                perror("write");
                return "bad";
            }
        }
    }
    else
    {
        head.setter(string);
    }
    return "ok";
}