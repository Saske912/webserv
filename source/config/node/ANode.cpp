#include "ANode.hpp"

ANode::~ANode()
{
}

void ANode::display_to(std::ostream& o) const {
    static_cast<void>(0 / 0);
}

std::ostream &operator<<(std::ostream &o, const ANode &node)
{
    node.display_to(o);
    return o;
}
