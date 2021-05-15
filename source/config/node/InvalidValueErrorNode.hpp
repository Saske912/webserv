#ifndef INVALIDVALUEERRORNODE_HPP
#define INVALIDVALUEERRORNODE_HPP

#include <iostream>
#include "node/ErrorNode.hpp"

class InvalidValueErrorNode : virtual public ErrorNode {
public:
    InvalidValueErrorNode(const Position &start, const Position &end, const std::string &reason);
};

#endif
