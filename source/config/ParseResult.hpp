#ifndef PARSERESULT_HPP
#define PARSERESULT_HPP

#include <iostream>
#include "Node.hpp"

class ParseResult
{
public:
	ParseResult();
	ParseResult(const ParseResult& other);

	Node* checkIn(const ParseResult& parseResult);
	ParseResult& success(Node* node);
	ParseResult& failure(ErrorNode *error);

	ErrorNode *error;
	Node *node;
};

#endif
