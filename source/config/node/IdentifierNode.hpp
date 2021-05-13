#ifndef IDENTIFIERNODE_HPP
#define IDENTIFIERNODE_HPP

#include <iostream>
#include "node/ANode.hpp"

class IdentifierNode : virtual public ANode {
public:
    explicit IdentifierNode(const Token &value);
    virtual void display_to(std::ostream &o) const;

    Token value;
};

#endif
