#include "RouteNode.hpp"
#include "Validator.hpp"

const ContextInfo RouteNode::validParamNames[] = {
    {"allowed_methods",   -1, Validator::validateAllowedMethods},
    {"root",              1,  Validator::validateRoot},
    {"directory_listing", 1,  Validator::validateDirectoryListing},
    {"index",             -1, Validator::validateIndex},
    {"cgi_extension",     2,  Validator::validateCgiExtension},
    {"upload_location",   1,  Validator::validateUploadLocation},

    {NULL,                0, NULL}
};

RouteNode::RouteNode(const Token &name, const Token &endpoint_, const ParamValuesType &values_)
    : name(name), endpoint(endpoint_), values(values_) {
}

RouteNode::RouteParamValidation RouteNode::getParamValue(const std::string &param) {
    for (int i = 0; validParamNames[i].paramName; ++i) {
        if (param == validParamNames[i].paramName) {
            return RouteParamValidation(1U << i);
        }
    }
    return RouteParamValidation(-1);
}

bool RouteNode::isValid(ParseResult &result) const {
    unsigned int validated = 0;

    for (ParamValuesType::const_iterator it = values.begin();
         it != values.end(); ++it) {
        RouteParamValidation current = getParamValue(it->name.value);
        if (current == RP_UNKNOWN) {
            result.failure(ErrorNode::getValueError(it->name, "Unknown Route Parameter."));
        }
        else if (validated & current) {
            result.failure(ErrorNode::getValueError(it->name, "Route parameters must not duplicate."));
        }
        else {
            validated |= current;
        }
    }
    if ((validated & RP_REQUIRED) != RP_REQUIRED) {
        result.failure(ErrorNode::getValueError(name, "'route' directive at least must have 'allowed_methods' and 'root' directives"));
    }
    return (validated & RP_REQUIRED) == RP_REQUIRED;
}

void RouteNode::displayTo(std::ostream &o) const {
    o << "(Route: " << endpoint << ": ";
    if (!values.empty()) {
        for (ParamValuesType::const_iterator it = values.begin();
             it != values.end(); ++it) {
            o << *it << ", ";
        }
        std::cout << "\b\b";
    }
    std::cout << ")";
}
