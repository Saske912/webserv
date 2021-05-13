#ifndef ERRORNODE_HPP
#define ERRORNODE_HPP

#include <iostream>
#include "node/ANode.hpp"

class ErrorNode : virtual public ANode {
public:
    ErrorNode(const Position &start, const Position &end, const std::string& reason);
    virtual std::string getType() const;
    const Position& getStart() const;
    const Position& getEnd() const;
    const std::string& getReason() const;

protected:
    Position start;
    Position end;
    std::string reason;
};

std::ostream& operator<<(std::ostream &o, const ErrorNode &errorNode);

#endif
