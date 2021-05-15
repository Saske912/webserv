#include "ParamNode.hpp"

ParamNode::ParamNode(const Token &name_, const ValuesType &values_) : name(name_), values(values_)
{
}

void ParamNode::displayTo(std::ostream &o) const
{
    o << "(Param: " << name << ": ";
    if (!values.empty()) {
        for (ValuesType::const_iterator it = values.begin();
             it != values.end(); ++it) {
            o << *it << ", ";
        }
        std::cout << "\b\b";
    }
    std::cout << ")";
}
