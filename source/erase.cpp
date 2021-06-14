//
// Created by Pamula File on 6/14/21.
//
#include "header.h"

void    erase(std::list<t_write>::iterator &it)
{
    std::list<std::string >::iterator iter = Header::current_files_in_work.begin();
    while (iter != Header::current_files_in_work.end() and *iter != it->head.getRequest())
        iter++;
    if (iter != Header::current_files_in_work.end())
        Header::current_files_in_work.erase(iter);
}
