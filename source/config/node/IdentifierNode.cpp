#include "IdentifierNode.hpp"

IdentifierNode::IdentifierNode(const Token &value_) : value(value_)
{
}

void IdentifierNode::display_to(std::ostream &o) const
{
    o << "(" << value << ")";
}
