#include <iostream>
#include <fcntl.h>
#include "Lexer.hpp"
#include "Node.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"

void print_tokens(std::vector<Token> const& tokens) {
	std::cout << "[size: " << tokens.size() << " | ";
	for (std::vector<Token>::const_iterator it = tokens.begin();
		 it < tokens.end(); ++it)
	{
		std::cout << *it << ", ";
	}
	std::cout << "\b\b]" << std::endl;
}

std::vector<Token> run(int fd) {
	Lexer lexer(fd);

	std::vector<Token> tokens = lexer.make_tokens();
	print_tokens(tokens);
	return tokens;
}

Node* run_parse(int fd) {
	std::vector<Token> tokens = run(fd);
	if (!tokens.empty() && tokens[0].type != Token::TT_EOF)
	{
		Parser parser(tokens);
		ParseResult result = parser.parse();
		if (result.error) {
			std::cout << *result.error;
			delete result.node;
			return NULL;
		}
		return result.node;
	}
	return NULL;
}

void run_interpret(int fd) {
	Node* node = run_parse(fd);
	if (node)
		std::cout << *node << std::endl;
	if (false && node) {
		std::cout << *node << std::endl;
		Interpreter interpreter;
		interpreter.visit(node);
	}
	delete node;
}

int main() {
	int fd = open("../ConfigExample.ws", O_RDONLY);

	if (fd == -1 ) {
		std::cout << "Config file not found!" << std::endl;
		return 1;
	}

	run_interpret(fd);

	return 0;
}
