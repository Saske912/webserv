#include "ErrorNode.hpp"

ErrorNode::ErrorNode(const Position &start, const Position &end,
                     const std::string &reason) : start(start), end(end), reason(reason)
{
}

std::string ErrorNode::getType() const
{
    return std::string("Unknown Error");
}

const Position &ErrorNode::getStart() const
{
    return start;
}

const Position &ErrorNode::getEnd() const
{
    return end;
}

const std::string &ErrorNode::getReason() const
{
    return reason;
}

std::ostream& operator<<(std::ostream& o, const ErrorNode& errorNode) {
    o << errorNode.getType() << ": " << errorNode.getReason() << std::endl <<
      "at " << errorNode.getStart().line << ":" << errorNode.getStart().col <<
      " (probably) until " << errorNode.getEnd().line << ":" << errorNode.getEnd().col <<
      std::endl;
    return o;
}
