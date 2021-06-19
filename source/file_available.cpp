//
// Created by Pamula File on 6/17/21.
//
#include "header.h"

bool    file_available(const std::string& request)
{
    return std::find(Header::current_files_in_work.begin(), \
        Header::current_files_in_work.end(), request) ==  Header::current_files_in_work.end();
}