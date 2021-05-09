#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <vector>
#include <exception>
#include <unistd.h>
#include "Token.hpp"
#include "Position.hpp"

class Lexer
{
	Lexer();
	Lexer(Lexer const& other);
	Lexer& operator=(Lexer const& other);
public:

	Lexer(int fd);
	~Lexer();

	void advance();
	std::vector<Token> make_tokens();
	void add_token(std::vector<Token>& tokens, Token::TokenType type);

	Token make_identifier();
	void skip_comment();

	static bool is_eof(char c);
	static bool is_space(char c);
	static bool is_identifier_char(char c);

	void read_next();

	int fd;
	Position pos;
	char current_char;

	char buf[1024];
	long bufpos;
	long bufmax;
};

#endif
