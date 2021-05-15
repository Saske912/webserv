#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include <iostream>
#include <list>
#include "Token.hpp"

class Validator {
    class IsNumberPredicate {
    public:
        bool operator()(const char &c);
    };
public:
    static bool isPath(const std::string &value);
    static bool isHost(const std::string &value);
    static bool isNumber(const std::string &value);
    static bool isMemory(const std::string &value);
    static bool isIP(const std::string &value);

    static unsigned int getMethodValue(const std::string &value);

    static const char* validateHost(const std::list<Token> &values);
    static const char* validatePort(const std::list<Token> &values);
    static const char* validateServerNames(const std::list<Token> &values);
    static const char* validateErrorPage(const std::list<Token> &values);
    static const char* validateClientMaxBodySize(const std::list<Token> &values);
    static const char* validateAllowedMethods(const std::list<Token> &values);
    static const char* validateRoot(const std::list<Token> &values);
    static const char* validateDirectoryListing(const std::list<Token> &values);
    static const char* validateIndex(const std::list<Token> &values);
    static const char* validateCgiExtension(const std::list<Token> &values);
    static const char* validateUploadLocation(const std::list<Token> &values);
};

#endif
