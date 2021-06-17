//
// Created by Pamula File on 6/17/21.
//
#include "header.h"

std::string get_http_line(std::string &buffer, Header &head)
{
    std::string line;
    size_t      finder;

    if ((finder = buffer.find("\r\n")) == std::string::npos)
    {
        head.reminder = buffer;
        throw std::exception();
    }
    line = buffer.erase(0, finder + 2);
    line.pop_back();
    line.pop_back();
    parse_request(line, head);
    return line;
}
