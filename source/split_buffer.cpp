//
// Created by Pamula File on 6/17/21.
//
#include "header.h"

std::string split_buffer( std::string &buffer, Header &head, server &serv )
{
    std::string     line;

    if (head.isClientNowInQueue())
    {
        return std::string();
    }
    line = get_http_line( buffer, head, serv );
    if (line == "body_end" || line.empty())
        return line;
    return split_buffer( buffer, head, serv );
}
