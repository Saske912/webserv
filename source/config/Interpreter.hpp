#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <iostream>
#include <exception>
#include <list>
#include "node/ANode.hpp"
#include "node/IdentifierNode.hpp"
#include "node/ParamNode.hpp"
#include "node/RouteNode.hpp"
#include "node/ServerNode.hpp"
#include "node/ConfigNode.hpp"
#include "Token.hpp"
#include "server.hpp"
#include "config.hpp"

class Interpreter {
    Interpreter &operator=(const Interpreter &other);

    int poor_atoi(std::string const &num);

public:
    class NoVisitMethodException : public std::exception {
    public:
        const char *what() const throw() { return "No valid visit method for node."; }
    };

    explicit Interpreter(config &config_);

    Interpreter(const Interpreter &other);

    ~Interpreter();

    void visit(ANode *node);

    void visit(IdentifierNode *node);

    void visit(ParamNode *node);

    route visit(RouteNode *node);

    void visit(ServerNode *node);

    void visit(ConfigNode *node);

    void add_error_page(server &serv, const std::list<Token> &values);

private:
    config &conf;

};

#endif
