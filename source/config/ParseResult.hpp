#ifndef PARSERESULT_HPP
#define PARSERESULT_HPP

#include <iostream>
#include "node/ErrorNode.hpp"

class ParseResult
{
public:
	ParseResult();
	ParseResult(const ParseResult& other);

	ANode* checkIn(const ParseResult& parseResult);
	ParseResult& success(ANode* node);
	ParseResult& failure(ErrorNode *error);

	ErrorNode *error;
	ANode     *node;
	bool      checkInSuccess;
};

#endif
