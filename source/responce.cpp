//
// Created by Pamula File on 6/30/21.
//
#include "header.h"

void response( Header &head, config &conf )
{
    std::string string;

    if (head.isBodyEnd() and !head.isClientNowInQueue() and head.isPermission())
    {
        if (head.isEmptyLine())
            buildHeader( head );
        if (head.getTransfer_Encoding() == "chunked" and !head.getError())
            sendFileChunked(  head.getFile( ), conf, head );
        else
            sendFile( head, conf );
    }
}
