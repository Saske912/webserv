//
// Created by Pamula File on 5/5/21.
//
#include "../header.h"

void error_exit(const std::string& str)
{
    std::cerr << str << ": " << strerror(errno) << std::endl;
    exit(errno);
}