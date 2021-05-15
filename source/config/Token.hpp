#ifndef PARSER_TOKEN_HPP
#define PARSER_TOKEN_HPP

#include <iostream>
#include "Position.hpp"

class Token {
public:
	static const char *tokens[];
	enum TokenType {
		NEWLINE, SEMICOLON, IDENTIFIER, COMMENT, LCURLY, RCURLY,
		LBRACE, RBRACE, COMMA,
		TT_EOF, UNEXPECTED
	};

	Token();
	explicit Token(TokenType type_, const Position* start_, const Position *end_ = NULL, const std::string &value_ = std::string());
	Token(const Token &other);
	Token& operator=(const Token &other);
	~Token();

	std::string text() const;

	TokenType type;
	std::string value;
	Position start;
	Position end;
};

std::ostream& operator<<(std::ostream &o, const Token& token);

#endif
