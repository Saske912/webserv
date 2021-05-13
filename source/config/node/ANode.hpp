#ifndef ANODE_HPP
#define ANODE_HPP

#include <iostream>
#include <list>
#include <exception>
#include "Token.hpp"
#include "Position.hpp"

class ANode
{
public:
    virtual ~ANode();
    virtual void display_to(std::ostream& o) const;
};

std::ostream& operator<<(std::ostream& o, ANode const& node);

#endif
