#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include <iostream>
#include <list>
#include "Token.hpp"

class Validator {
public:
    static bool isPath(const std::string &value);
    static bool isNumber(const std::string &value);
    static bool isMemory(const std::string &value);
    static bool isIP(const std::string &value);

    static unsigned int getMethodValue(const std::string &value);

    static char* validateHost(const std::list<Token> &values);
    static char* validatePort(const std::list<Token> &values);
    static char* validateServerNames(const std::list<Token> &values);
    static char* validateErrorPage(const std::list<Token> &values);
    static char* validateClientMaxBodySize(const std::list<Token> &values);
    static char* validateAllowedMethods(const std::list<Token> &values);
    static char* validateRoot(const std::list<Token> &values);
    static char* validateDirectoryListing(const std::list<Token> &values);
    static char* validateIndex(const std::list<Token> &values);
    static char* validateCgiExtension(const std::list<Token> &values);
    static char* validateUploadLocation(const std::list<Token> &values);
};

#endif
