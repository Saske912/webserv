#include "InvalidSyntaxErrorNode.hpp"

InvalidSyntaxErrorNode::InvalidSyntaxErrorNode(const Position &start,
                                               const Position &end,
                                               const std::string &reason)
    : ErrorNode(start, end, reason, "Invalid Syntax Error") {
}
