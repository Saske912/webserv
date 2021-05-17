#include "ANode.hpp"

ANode::~ANode() {
}

void ANode::displayTo(std::ostream &) const {
    throw std::exception();
}

std::ostream &operator<<(std::ostream &o, const ANode &node) {
    node.displayTo(o);
    return o;
}
