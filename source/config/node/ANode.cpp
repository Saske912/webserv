#include "ANode.hpp"

ANode::~ANode()
{
}

void ANode::display_to(std::ostream&) const {
    throw std::exception();
}

std::ostream &operator<<(std::ostream &o, const ANode &node)
{
    node.display_to(o);
    return o;
}
