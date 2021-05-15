#include "IdentifierNode.hpp"

IdentifierNode::IdentifierNode(const Token &value_) : value(value_) {
}

void IdentifierNode::displayTo(std::ostream &o) const {
    o << "(" << value << ")";
}
