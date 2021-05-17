#ifndef PARAMNODE_HPP
#define PARAMNODE_HPP

#include <iostream>
#include "node/ANode.hpp"

class ParamNode : virtual public ANode {
public:
    typedef std::list<Token> ValuesType;

    ParamNode(const Token &name_, const ValuesType &values_);

    virtual void displayTo(std::ostream &o) const;

    Token      name;
    ValuesType values;
};

#endif
