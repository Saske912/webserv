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

config parse(int fd, const char *filename) {
    std::vector<Token> tokens = Lexer(fd, filename).make_tokens();
    ParseResult ast = Parser(tokens).config();
    if (ast.error) {
        std::cout << ast.error << std::endl;
        exit(1);
    }
    config result;
    Interpreter(result).visit(ast.node);
    delete ast.node;
    delete ast.error;
    return result;
}

config parse(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (-1 == fd) {
        error_exit(std::string("Error opening config file '") + filename + "'");
    }
    config result = parse(fd, filename);
    close(fd);
    return result;
}
