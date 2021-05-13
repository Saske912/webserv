#include "RouteNode.hpp"

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
