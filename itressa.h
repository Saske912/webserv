#ifndef ITRESSA_H
#define ITRESSA_H

#include "config.hpp"
#include "Token.hpp"

typedef const char* (*param_validator)(const std::list<Token> &values);

struct ContextInfo {
    const char *paramName;
    int numberOfParams;
    param_validator validate;
};

config parse(int fd);
config parse(const char *filename);

#endif
