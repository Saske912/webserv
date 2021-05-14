#ifndef INVALIDSYNTAXERRORNODE_HPP
#define INVALIDSYNTAXERRORNODE_HPP

#include <iostream>
#include "node/ErrorNode.hpp"

class InvalidSyntaxErrorNode : virtual public ErrorNode {
public:
    InvalidSyntaxErrorNode(const Position &start, const Position &end, const std::string &reason);
};

#endif
