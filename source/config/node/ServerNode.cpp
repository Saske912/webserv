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
