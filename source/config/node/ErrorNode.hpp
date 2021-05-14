#ifndef ERRORNODE_HPP
#define ERRORNODE_HPP

#include <iostream>
#include "node/ANode.hpp"

class ErrorNode : virtual public ANode {
public:
    ErrorNode(const Position &start, const Position &end, const std::string& reason, const char *type_);
    const Position& getStart() const;
    const Position& getEnd() const;
    const std::string& getReason() const;
    const char* getType() const;
    const ErrorNode* getNext() const;
    void addNext(ErrorNode *node);

protected:
    Position start;
    Position end;
    std::string reason;
    const char *type;
    ErrorNode *next;
};

std::ostream& operator<<(std::ostream &o, const ErrorNode *errorNode);

#endif
