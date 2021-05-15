#include "ConfigNode.hpp"

ConfigNode::ConfigNode(const ServerValuesType &servers_) : servers(servers_)
{
}

void ConfigNode::displayTo(std::ostream &o) const
{
    o << "(Config: ";
    if (!servers.empty()) {
        for (ServerValuesType::const_iterator it = servers.begin();
             it != servers.end(); ++it) {
            o << *it << ", ";
        }
        std::cout << "\b\b";
    }
    std::cout << ")";
}
