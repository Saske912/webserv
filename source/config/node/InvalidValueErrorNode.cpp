#include "InvalidValueErrorNode.hpp"

InvalidValueErrorNode::InvalidValueErrorNode(const Position &start,
                                               const Position &end,
                                               const std::string &reason)
    : ErrorNode(start, end, reason, "Invalid Value Error")
{
}
