//
// Created by Pamula File on 6/30/21.
//
#include "header.h"

void communication_with_clients( std::list<Header> &set, server &serv, fd_set *clients_with_data, config &conf )
{
    int             opt;
    socklen_t len = sizeof(opt);

    std::list<Header>::iterator it = set.begin();
    while (it != set.end())
    {
        getsockopt( it->getClient( ), SOL_SOCKET, SO_ERROR, &opt, &len );
        if (opt == ECONNRESET)
        {
            it = update_descriptors( it->getRealPathToFile( ), it, set, conf );
            continue ;
        }
        if (opt)
            strerror(opt);
        if ( receive( it, serv, clients_with_data ))
        {
            it = update_descriptors( it->getRealPathToFile( ), it, set, conf );
            continue ;
        }
        response( *it, conf );
        if (it->isClientNowInQueue())
            it = set.erase(it);
        else if (it != set.end())
            it++;
    }
}

void communication_with_client(Header &head, server &serv, config & conf)
{
    int             opt;
    socklen_t len = sizeof(opt);

    head.setPermission(true);
    Header::current_files_in_work.push_back(head.getRealPathToFile());
    getsockopt( head.getClient( ), SOL_SOCKET, SO_ERROR, &opt, &len );
    if (opt == ECONNRESET)
    {
        close(head.getClient());
        update_descriptors( head.getRealPathToFile( ), head, conf );
        return ;
    }
    if (head.getBuffer().length())
    {
        split_buffer(head.getBuffer(), head, serv);
    }
    response(head, conf);
}