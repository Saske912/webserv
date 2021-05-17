#ifndef SERVERNODE_HPP
#define SERVERNODE_HPP

#include <iostream>
#include "node/ParamNode.hpp"
#include "node/RouteNode.hpp"

class ServerNode : virtual public ANode {
public:
    typedef std::list<ParamNode> ParamValuesType;
    typedef std::list<RouteNode> RouteValuesType;

    enum ServerParamValidation {
        SP_UNKNOWN              = -1,
        SP_HOST                 = 1U << 0,
        SP_PORT                 = 1U << 1,
        SP_SERVER_NAMES         = 1U << 2,
        SP_ERROR_PAGE           = 1U << 3,
        SP_CLIENT_MAX_BODY_SIZE = 1U << 4,
        SP_REQUIRED             = SP_HOST | SP_PORT | SP_CLIENT_MAX_BODY_SIZE
    };

    ServerNode(const ParamValuesType &params_, const RouteValuesType &routes_);

    virtual void displayTo(std::ostream &o) const;

    static ServerParamValidation getParamValue(const std::string &param);

    bool isValid() const;

    static const ContextInfo validParamNames[];

    ParamValuesType params;
    RouteValuesType routes;
};

#endif
