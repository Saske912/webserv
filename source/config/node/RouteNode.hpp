#ifndef ROUTENODE_HPP
#define ROUTENODE_HPP

#include <iostream>
#include "node/ParamNode.hpp"

class RouteNode : virtual public ANode {
public:
    typedef std::list<ParamNode> ParamValuesType;

    enum RouteParamValidation {
        RP_UNKNOWN              = -1,
        RP_ALLOWED_METHODS      = 1U << 0,
        RP_ROOT                 = 1U << 1,
        RP_DIRECTORY_LISTING    = 1U << 2,
        RP_INDEX                = 1U << 3,
        RP_CGI_EXTENSION        = 1U << 4,
        RP_UPLOAD_LOCATION      = 1U << 5,
        RP_REQUIRED = RP_ALLOWED_METHODS | RP_ROOT | RP_INDEX
    };

    explicit RouteNode(const Token &endpoint, const ParamValuesType &values_);
    virtual void displayTo(std::ostream &o) const;
    static RouteParamValidation getParamValue(const std::string &param);
    bool isValid() const;

    static const ContextInfo validParamNames[];

    Token endpoint;
    ParamValuesType values;
};

#endif
