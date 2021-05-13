#ifndef SERVERNODE_HPP
#define SERVERNODE_HPP

#include <iostream>
#include "node/ParamNode.hpp"
#include "node/RouteNode.hpp"

class ServerNode : virtual public ANode {
public:
    typedef std::list<ParamNode> ParamValuesType;
    typedef std::list<RouteNode> RouteValuesType;

    ServerNode(const ParamValuesType &params_, const RouteValuesType &routes_);
    virtual void display_to(std::ostream &o) const;

    ParamValuesType params;
    RouteValuesType routes;
};

#endif
