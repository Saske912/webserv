//
// Created by Pamula File on 6/17/21.
//
#include "header.h"

std::string get_http_line(std::string &buffer, Header &head, config &conf)
{
    std::string line;
    size_t      finder;

    if ((finder = buffer.find("\r\n")) == std::string::npos)
    {
        head.setReminder(buffer);
        throw std::exception();
    }
    line = buffer.substr(0, finder);
    buffer.erase(0, finder + 2);
    return parse_request(line, head, conf);
}
