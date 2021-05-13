#ifndef ROUTENODE_HPP
#define ROUTENODE_HPP

#include <iostream>
#include "node/ParamNode.hpp"

class RouteNode : virtual public ANode {
public:
    typedef std::list<ParamNode> ParamValuesType;

    explicit RouteNode(const Token &endpoint, const ParamValuesType &values_);
    virtual void display_to(std::ostream &o) const;

    Token endpoint;
    ParamValuesType values;
};

#endif
