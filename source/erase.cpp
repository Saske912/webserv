//
// Created by Pamula File on 6/14/21.
//
#include "header.h"

void erase( std::string const &realPathToFile, Header &head)
{
    std::list<std::string >::iterator iter = Header::current_files_in_work.begin();
    while (iter != Header::current_files_in_work.end() and *iter != realPathToFile)
        iter++;
    if (iter != Header::current_files_in_work.end())
        Header::current_files_in_work.erase(iter);
    head.eraseStruct();
//        resetIt(it);
}
