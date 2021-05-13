#include "ParseResult.hpp"

ParseResult::ParseResult() : error(), node()
{
}

ParseResult::ParseResult(const ParseResult &other) : error(other.error), node(other.node)
{
}

ANode* ParseResult::checkIn(const ParseResult &parseResult)
{
	if (parseResult.error) {
		error = parseResult.error;
	}
	return parseResult.node;
}

ParseResult& ParseResult::success(ANode* node_)
{
	node = node_;
	return *this;
}

ParseResult& ParseResult::failure(ErrorNode *error_)
{
	error = error_;
	return *this;
}
