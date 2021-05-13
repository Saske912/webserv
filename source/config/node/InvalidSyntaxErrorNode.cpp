#include "InvalidSyntaxErrorNode.hpp"

InvalidSyntaxErrorNode::InvalidSyntaxErrorNode(const Position &start,
                                               const Position &end,
                                               const std::string &reason)
    : ErrorNode(start, end, reason)
{
}

std::string InvalidSyntaxErrorNode::get_type() const
{
    return "Invalid Syntax";
}
