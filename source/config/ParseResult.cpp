#include "ParseResult.hpp"

ParseResult::ParseResult() : error(), node(), checkInSuccess(true)
{
}

ParseResult::ParseResult(const ParseResult &other)
    : error(other.error), node(other.node), checkInSuccess(other.checkInSuccess)
{
}

ANode* ParseResult::checkIn(const ParseResult &parseResult)
{
    checkInSuccess = parseResult.checkInSuccess;
	if (!checkInSuccess) {
	    if (!error)
		    error = parseResult.error;
	    else
		    error->addNext(parseResult.error);
	}
	return parseResult.node;
}

ParseResult& ParseResult::success(ANode* node_)
{
	node = node_;
    checkInSuccess = true;
	return *this;
}

ParseResult& ParseResult::failure(ErrorNode *error_)
{
    if (!error)
	    error = error_;
    else
        error->addNext(error_);
    checkInSuccess = false;
	return *this;
}
