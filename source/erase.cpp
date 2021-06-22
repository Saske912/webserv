//
// Created by Pamula File on 6/14/21.
//
#include "header.h"

std::list<Header>::iterator& erase( std::list<Header>::iterator &it, std::list<Header> &set )
{
    close(it->getClient());
    std::list<std::string >::iterator iter = Header::current_files_in_work.begin();
    while (iter != Header::current_files_in_work.end() and *iter != it->getRequest())
        iter++;
    if (iter != Header::current_files_in_work.end())
        Header::current_files_in_work.erase(iter);
    it = set.erase(it);
    return it;
//        it->eraseStruct();
//        resetIt(it);
}
