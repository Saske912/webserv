#include "Validator.hpp"

bool Validator::isPath(const std::string &) {
    return false;
}

bool Validator::isNumber(const std::string &) {
    return false;
}

bool Validator::isMemory(const std::string &) {
    return false;
}

bool Validator::isIP(const std::string &) {
    return false;
}

unsigned int Validator::getMethodValue(const std::string &) {
    return 0;
}

char *Validator::validateHost(const std::list<Token> &) {
    return NULL;
}

char *Validator::validatePort(const std::list<Token> &) {
    return NULL;
}

char *Validator::validateServerNames(const std::list<Token> &) {
    return NULL;
}

char *Validator::validateErrorPage(const std::list<Token> &) {
    return NULL;
}

char *Validator::validateClientMaxBodySize(const std::list<Token> &) {
    return NULL;
}

char *Validator::validateAllowedMethods(const std::list<Token> &) {
    return NULL;
}

char *Validator::validateRoot(const std::list<Token> &) {
    return NULL;
}

char *Validator::validateDirectoryListing(const std::list<Token> &) {
    return NULL;
}

char *Validator::validateIndex(const std::list<Token> &) {
    return NULL;
}

char *Validator::validateCgiExtension(const std::list<Token> &) {
    return NULL;
}

char *Validator::validateUploadLocation(const std::list<Token> &) {
    return NULL;
}
