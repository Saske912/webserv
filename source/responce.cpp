//
// Created by Pamula File on 6/30/21.
//
#include "header.h"
//
//void response( std::list<Header>::iterator &it, config &conf )
//{
//    std::string string;
//
//    if (it->isBodyEnd() and !it->isClientNowInQueue() and it->isPermission())
//    {
//        if (it->isEmptyLine())
//            buildHeader( *it );
//        if (it->getTransfer_Encoding() == "chunked" and !it->getError())
//            sendFileChunked( it->getFile( ), conf,*it );
//        else
//            sendFile( *it, conf );
//    }
//}

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
