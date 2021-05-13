#include "header.h"
#include <iostream>
#include <vector>
#include <fcntl.h>
#include "Token.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"
#include "ParseResult.hpp"
#include "config.hpp"

config parse(int fd) {
    std::vector<Token> tokens = Lexer(fd).make_tokens();
    ParseResult ast = Parser(tokens).config();
    if (ast.error) {
        std::cout << ast.error << std::endl;
        error_exit(ast.error->getReason());
    }
    config result;
    Interpreter(result).visit(ast.node);
    delete ast.node;
    delete ast.error;
    return result;
}

config parse(char *filename) {
    int fd = open(filename, O_RDONLY);

    return parse(fd);
}
