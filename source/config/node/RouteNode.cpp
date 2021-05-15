#include "RouteNode.hpp"
#include "Validator.hpp"

const ContextInfo RouteNode::validParamNames[] = {
    {"allowed_methods", -1, Validator::validateAllowedMethods},
    {"root", 1, Validator::validateRoot},
    {"directory_listing", 1, Validator::validateDirectoryListing},
    {"index", -1, Validator::validateIndex},
    {"cgi_extension", 2, Validator::validateCgiExtension},
    {"upload_location", 1, Validator::validateUploadLocation},

    {NULL, 0, NULL}
};

RouteNode::RouteNode(const Token &endpoint_, const ParamValuesType &values_) : endpoint(endpoint_), values(values_)
{
}

void RouteNode::display_to(std::ostream &o) const
{
    o << "(Route: " << endpoint << ": ";
    if (!values.empty()) {
        for (ParamValuesType ::const_iterator it = values.begin();
             it != values.end(); ++it) {
            o << *it << ", ";
        }
        std::cout << "\b\b";
    }
    std::cout << ")";
}
