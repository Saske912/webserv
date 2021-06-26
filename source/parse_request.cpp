//
// Created by Pamula File on 6/17/21.
//
#include "header.h"
#include "Number.hpp"

std::string parse_request( const std::string &string, Header &head, server &serv )
{
    if (head.isEmptyLine())
    {
        if (!head.getFile( ))
        {
            head.setFile(head.getServ()->descriptorForReceive(head));
        }
        if (head.getTransfer_Encoding() == "chunked")
        {
            if (string == "0"){
                head.setBodyEnd(true);
                if (head.getBodySize() > head.getRout()->get_client_max_body_size())
                {
                    head.setError(413);
                    head.setFile(serv.exception_processing(413, head));
                }
                return "body_end";
            }
            if (string.find_last_not_of("0123456789abcdef") != std::string::npos)
            {
                head.setBodySize(head.getBodySize() + static_cast<int>(string.length()));
                if ( write( head.getFile( ), string.c_str(), string.length()) != static_cast<ssize_t>(string.length()))
                {
                    perror("write");
                    return "bad";
                }
            }
        }
        else
        {
            if (string.empty())
            {
                head.setBodyEnd(true);
                if (head.getBodySize() > head.getRout()->get_client_max_body_size())
                {
                    head.setError(413);
                    head.setFile(serv.exception_processing(413, head));
                }
                return "body_end";
            }
            else if ( write( head.getFile( ), string.c_str(), string.length()) != static_cast<ssize_t>(string.length()))
                error_exit("write in 2 if parse_request");
            head.setBodySize(head.getBodySize() + static_cast<int>(string.length()));
        }
    }
    else
        head.setter( string, serv );
    return "ok";
}