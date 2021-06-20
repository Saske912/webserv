//
// Created by Pamula File on 6/17/21.
//
#include "header.h"

std::string     split_buffer(std::string &buffer, Header &head, config &conf)
{
    std::string     line;

    try
    {
        line = get_http_line(buffer, head, conf);
        if (line == "body_end")
            return line;
    }
    catch (std::exception &)
    {
        return line;
    }
    return split_buffer(buffer, head, conf);
}
