#ifndef CONFIGNODE_HPP
#define CONFIGNODE_HPP

#include <iostream>
#include "node/ParamNode.hpp"
#include "node/ServerNode.hpp"

class ConfigNode : virtual public ANode {
public:
    typedef std::list<ParamNode> ParamValuesType;
    typedef std::list<ServerNode> ServerValuesType;

    ConfigNode(const ServerValuesType &servers_);
    virtual void display_to(std::ostream &o) const;

    ServerValuesType servers;
};

#endif
