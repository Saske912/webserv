#include "ErrorNode.hpp"

ErrorNode::ErrorNode(const Position &start, const Position &end,
                     const std::string &reason, const char *type_)
     : start(start), end(end), reason(reason), type(type_), next()
{
}

const char* ErrorNode::getType() const
{
    return type;
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

const ErrorNode *ErrorNode::getNext() const {
    return next;
}

std::ostream& operator<<(std::ostream& o, const ErrorNode *errorNode) {
    o << errorNode->getType() << ": " << std::endl << "\t " << errorNode->getReason() << std::endl <<
      "\t at " << errorNode->getStart().filename << ":" <<
      errorNode->getStart().line << ":" << errorNode->getStart().col <<
      std::endl;
    if (errorNode->getNext() != NULL) {
        o << errorNode->getNext();
    }
    return o;
}

void ErrorNode::addNext(ErrorNode *node) {
    ErrorNode *last;

    last = this;
    while (last->next != NULL) {
        last = last->next;
    }
    last->next = node;
}
