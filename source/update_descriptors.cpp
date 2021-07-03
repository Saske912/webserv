//
// Created by Pamula File on 6/14/21.
//
#include "header.h"

std::list<Header>::iterator
update_descriptors( std::string const &realPathToFile, std::list<Header>::iterator &it, std::list<Header> &set,
                    config &conf )
{
    std::list<std::string >::iterator iter = Header::current_files_in_work.begin();
    std::list<std::string >::iterator iter_end = Header::current_files_in_work.end();

    while (iter != iter_end)
    {
        if (*iter == realPathToFile)
        {
            Header::current_files_in_work.erase(iter);
            conf.moveFromWait(realPathToFile);
            break ;
        }
        iter++;
    }
    FD_CLR(it->getClient(), &conf.write_set);
    close(it->getClient());
    it->eraseStruct();
    it = set.erase(it);
    return it;
}

void update_descriptors( std::string const &realPathToFile, Header &head, config &conf )
{
    std::list<std::string >::iterator iter = Header::current_files_in_work.begin();
    std::list<std::string >::iterator iter_end = Header::current_files_in_work.end();

    while (iter != iter_end)
    {
        if (*iter == realPathToFile)
        {
            Header::current_files_in_work.erase(iter);
            conf.moveFromWait(realPathToFile);
            break ;
        }
        iter++;
    }
    FD_CLR(head.getClient(), &conf.write_set);
    head.eraseStruct();
}
