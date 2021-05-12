//
// Created by Pamula File on 5/9/21.
//
#ifndef WPERSIMM_H
#define WPERSIMM_H
#include <iostream>
#include "source/header.hpp"

std::string &rtrim(std::string &s, char *t);
std::string &ltrim(std::string &s, char *t);
std::string &trim(std::string &s, char *t);
extern "C"
{
#include "libft/libft.h"
}

#endif //WPERSIMM_H
