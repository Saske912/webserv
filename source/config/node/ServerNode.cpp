#include "ServerNode.hpp"
#include "Validator.hpp"

const ContextInfo ServerNode::validParamNames[] = {
        {"host", 1, Validator::validateHost},
        {"port", 1, Validator::validatePort},
        {"server_names", -1, Validator::validateServerNames},
        {"error_page", 2, Validator::validateErrorPage},
        {"client_max_body_size", 1, Validator::validateClientMaxBodySize},

        {NULL, 0, NULL}
};

ServerNode::ServerNode(const ParamValuesType &params_,
                       const RouteValuesType &routes_) : params(params_), routes(routes_)
{
}

ServerNode::ServerParamValidation ServerNode::getParamValue(const std::string &param) {
    for (int i = 0; validParamNames[i].paramName; ++i) {
        if (param == validParamNames[i].paramName) {
            return ServerParamValidation(1U << i);
        }
    }
    return ServerParamValidation(-1);
}

bool ServerNode::isValid() const {
    if (routes.empty())
        return false;
    unsigned int validated = 0;
    for (ParamValuesType::const_iterator it = params.begin();
         it != params.end(); ++it) {
        ServerParamValidation current = getParamValue(it->name.value);
        if (current == SP_UNKNOWN || validated & current) {
            return false;
        }
        if (current != SP_ERROR_PAGE) // may be multiple times for different codes
            validated |= current;
    }
    return (validated & SP_REQUIRED) == SP_REQUIRED;
}

void ServerNode::displayTo(std::ostream &o) const
{
    o << "(Server: ";
    if (!params.empty()) {
        for (ParamValuesType ::const_iterator it = params.begin();
             it != params.end(); ++it) {
            o << *it << ", ";
        }
        std::cout << "\b\b";
    }
    o << " | ";
    if (!routes.empty()) {
        for (RouteValuesType::const_iterator it = routes.begin();
             it != routes.end(); ++it) {
            o << *it << ", ";
        }
        std::cout << "\b\b";
    }
    std::cout << ")";
}
