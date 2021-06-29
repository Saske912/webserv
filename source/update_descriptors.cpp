//
// Created by Pamula File on 6/14/21.
//
#include "header.h"

std::list<Header>::iterator
update_descriptors( std::string const &realPathToFile, std::list<Header>::iterator &it, std::list<Header> &set )
{
    std::list<std::string >::iterator iter = Header::current_files_in_work.begin();
    std::list<std::string >::iterator iter_end = Header::current_files_in_work.end();

    while (iter != iter_end)
    {
        if (*iter == realPathToFile)
        {
            Header::current_files_in_work.erase(iter);
            it->getServ()->getConf()->moveFromWait(realPathToFile);
            break ;
        }
        iter++;
    }
    it->eraseStruct();
    close(it->getClient());
    it = set.erase(it);
    return it;
}

void    update_descriptors( std::string const &realPathToFile, Header &head)
{
    std::list<std::string >::iterator iter = Header::current_files_in_work.begin();
    std::list<std::string >::iterator iter_end = Header::current_files_in_work.end();

    while (iter != iter_end)
    {
        if (*iter == realPathToFile)
        {
            Header::current_files_in_work.erase(iter);
            head.getServ()->getConf()->moveFromWait(realPathToFile);
            break ;
        }
        iter++;
    }
    head.eraseStruct();
}
