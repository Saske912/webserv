#include "ServerNode.hpp"

ServerNode::ServerNode(const ParamValuesType &params_,
                       const RouteValuesType &routes_) : params(params_), routes(routes_)
{
}

void ServerNode::display_to(std::ostream &o) const
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
