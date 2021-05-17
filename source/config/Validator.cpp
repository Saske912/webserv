#include "Validator.hpp"
#include <algorithm>
#include <sys/stat.h>

bool Validator::IsNumberPredicate::operator()(const char &c) {
    return c >= '0' && c <= '9';
}

bool Validator::isPath(const std::string &) {
    return true;
}

bool Validator::isHost(const std::string &) {
    return true;
}

bool Validator::isNumber(const std::string &value) {
    return std::all_of(value.begin(), value.end(), IsNumberPredicate());
}

bool Validator::isMemory(const std::string &value) {
    IsNumberPredicate      isDigit;
    std::string::size_type i = 0;
    while (i < value.size() && isDigit(value[i])) {
        ++i;
    }
    if (i == 0 || value.size() > i + 1)
        return false;
    if (i == value.size() ||
        std::string("KMG").find(value[i]) != std::string::npos) {
        return true;
    }
    return false;
}

bool Validator::isIP(const std::string &) {
    return true;
}

unsigned int Validator::getMethodValue(const std::string &method) {
    static const char *methods[] = {"GET", "HEAD", "POST", "PUT",
                                    "DELETE", "CONNECT", "OPTIONS", "TRACE", "PATCH", NULL};

    for (int i = 0; methods[i]; ++i) {
        if (method == methods[i])
            return i + 1;
    }
    return -1;
}

const char *Validator::validateHost(const std::list<Token> &values) {
    if (!values.empty()) {
        std::string value = values.front().value;
        if (!isIP(value.c_str()) && !isHost(value.c_str())) {
            return "Host is not valid";
        }
    }
    return NULL;
}

const char *Validator::validatePort(const std::list<Token> &values) {
    if (!values.empty() && !isNumber(values.front().value)) {
        return "Port must be number";
    }
    return NULL;
}

const char *Validator::validateServerNames(const std::list<Token> &values) {
    for (std::list<Token>::const_iterator it = values.begin();
         it != values.end(); ++it) {
        if (!isHost(it->value)) {
            return "Invalid server name";
        }
    }
    return NULL;
}

const char *Validator::validateErrorPage(const std::list<Token> &values) {
    if (values.size() > 1) {
        std::list<Token>::const_iterator it = values.begin();
        if (!isNumber(it->value)) {
            return "error_page must be followed by integer error code";
        }
        ++it;
        if (it->value.empty()) {
            return "error_page filename is empty";
        }
        struct stat filestat;
        if (-1 == stat(it->value.c_str(), &filestat) || !(filestat.st_mode & (S_IFREG))) {
            return "Invalid file for error_page";
        }
    }
    return NULL;
}

const char *Validator::validateClientMaxBodySize(const std::list<Token> &values) {
    if (!values.empty() && !isMemory(values.front().value)) {
        return "Invalid client max body size value";
    }
    return NULL;
}

const char *Validator::validateAllowedMethods(const std::list<Token> &values) {
    if (!values.empty()) {
        unsigned int                          validated = 0;
        for (std::list<Token>::const_iterator it        = values.begin();
             it != values.end(); ++it) {
            int current = getMethodValue(it->value);
            if (current == -1) {
                return "Unknown HTTP method";
            }
            else if (validated & (1U << current)) {
                return "Duplicated HTTP method";
            }
            validated |= 1U << current;
        }
    }
    return NULL;
}

const char *Validator::validateRoot(const std::list<Token> &) {
    return NULL;
}

const char *Validator::validateDirectoryListing(const std::list<Token> &values) {
    if (!values.empty() && values.front().value != "on" && values.front().value != "off") {
        return "directory_listing can be only 'on' or 'off'";
    }
    return NULL;
}

const char *Validator::validateIndex(const std::list<Token> &) {
    return NULL;
}

const char *Validator::validateCgiExtension(const std::list<Token> &) {
    return NULL;
}

const char *Validator::validateUploadLocation(const std::list<Token> &) {
    return NULL;
}
