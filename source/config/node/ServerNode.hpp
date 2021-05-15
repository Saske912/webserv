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
    virtual void displayTo(std::ostream &o) const;

    static const ContextInfo validParamNames[];

    ParamValuesType params;
    RouteValuesType routes;
};

#endif
