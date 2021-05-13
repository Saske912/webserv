#include "Parser.hpp"

Parser::Parser(std::vector<Token> &tokens_) : tokens(tokens_), current_token(), tok_idx(-1)
{
	advance();
}

Token &Parser::advance()
{
	++tok_idx;
	if (static_cast<std::vector<Token>::size_type>(tok_idx) < tokens.size()) {
		current_token = &tokens[tok_idx];
	}
	return *current_token;
}

ParseResult Parser::parse()
{
	ParseResult result = config();
	if (!result.error && current_token->type != Token::TT_EOF) {
		return result.failure(getSyntaxError("Unknown syntax error"));
	}
	return result;
}

ParseResult Parser::config() {
	ParseResult result;
	ConfigNode::ServerValuesType servers;
	while (current_token->type == Token::IDENTIFIER) {
		if (current_token->value == "server") {
			advance();
			ANode *serv = result.checkIn(server());
			if (result.error)
			{
				delete serv;
				return result;
			}
			servers.push_back(*dynamic_cast<ServerNode *>(serv));
			delete serv;
		} else {
		    return result.failure(getSyntaxError("Wrong Identifier name"));
		}
		skip_end_of_line_tokens();
	}
	return result.success(new ConfigNode(servers));
}

ParseResult Parser::server()
{
	ParseResult result;
	ServerNode::ParamValuesType params;
	ServerNode::RouteValuesType routes;
	if (expect_lcurly(result))
		return result;
	while (current_token->type == Token::IDENTIFIER) {
		if (current_token->value == "route") {
			advance();
			ANode *route_ = result.checkIn(route());
			if (result.error)
			{
				delete route_;
				return result;
			}
			routes.push_back(*dynamic_cast<RouteNode *>(route_));
			delete route_;
		} else {
			ANode *param_ = result.checkIn(param());
			if (result.error) {
				delete param_;
				return result;
			}
			params.push_back(*dynamic_cast<ParamNode*>(param_));
			delete param_;
		}
		skip_end_of_line_tokens();
	}
	if (expect_rcurly(result))
		return result;
	return result.success(new ServerNode(params, routes));
}

ParseResult Parser::route()
{
	ParseResult result;
	Token endpoint;
	RouteNode::ParamValuesType params;
	if (current_token->type == Token::IDENTIFIER) {
		endpoint = *current_token;
		advance();
	}
	else {
		return result.failure(getSyntaxError("Expected route path."));
	}
	if (expect_lcurly(result))
		return result;
	while (current_token->type == Token::IDENTIFIER) {
		ANode *param_ = result.checkIn(param());
		if (result.error) {
			delete param_;
			return result;
		}
		params.push_back(*dynamic_cast<ParamNode*>(param_));
		delete param_;
	}
	if (expect_rcurly(result))
		return result;
	return result.success(new RouteNode(endpoint, params));
}

ParseResult Parser::param()
{
	ParseResult result;
	Token name;
	ParamNode::ValuesType values;

	if (current_token->type == Token::IDENTIFIER) { // todo value must be in param names set
		name = *current_token;
		advance();
	}
	else {
		return result.failure(getSyntaxError("Expected param name identifier"));
	}

	//todo get list properly
	while (current_token->type == Token::COMMA || current_token->type == Token::LBRACE || current_token->type == Token::RBRACE)
		advance();
	while (current_token->type == Token::IDENTIFIER) {
		values.push_back(*current_token);
		advance();
		//todo get list properly
		while (current_token->type == Token::COMMA || current_token->type == Token::LBRACE || current_token->type == Token::RBRACE)
			advance();
	}
	skip_end_of_line_tokens();

	return result.success(new ParamNode(name, values));
}

bool Parser::expect_lcurly(ParseResult& result)
{
	skip_end_of_line_tokens();
	if (current_token->type == Token::LCURLY)
	{
		advance();
		skip_end_of_line_tokens();
		return false;
	}
	skip_end_of_line_tokens();
	result.failure(getSyntaxError("Expected '{'"));
	return true;
}

bool Parser::expect_rcurly(ParseResult& result)
{
	skip_end_of_line_tokens();
	if (current_token->type == Token::RCURLY) {
		advance();
		skip_end_of_line_tokens();
		return false;
	}
	skip_end_of_line_tokens();
	result.failure(getSyntaxError("Expected '}'"));
	return true;
}

InvalidSyntaxErrorNode *Parser::getSyntaxError(const std::string &reason)
{
	return new InvalidSyntaxErrorNode(current_token->start, current_token->end, reason);
}

void Parser::skip_end_of_line_tokens()
{
	while (current_token->type == Token::NEWLINE ||
		current_token->type == Token::COMMENT ||
		current_token->type == Token::SEMICOLON) {
		advance();
	}
}
