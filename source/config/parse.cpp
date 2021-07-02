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

bool is_config_file_invalid(const char *filename) {
    struct stat st;
    if (-1 == stat(filename, &st)) {
        return true;
    }
    if (st.st_mode & S_IFMT & S_IFDIR) {
        errno = EISDIR;
        return true;
    }
    return false;
}

std::size_t check_tokens_unexpected(std::vector<Token> &token) {
    std::size_t                       found = 0;
    for (std::vector<Token>::iterator it    = token.begin();
         it != token.end(); ++it) {
        if (it->type == Token::UNEXPECTED) {
            ErrorNode *err = ErrorNode::getSyntaxError(*it, "Unexpected symbol " + it->text());
            std::cout << err;
            delete err;
            ++found;
        }
    }
    return found;
}

config parse(int fd, const char *filename) {
    std::vector<Token> tokens = Lexer(fd, filename).make_tokens();
    if (std::size_t errors = check_tokens_unexpected(tokens)) {
        if (errors + 1 >= tokens.size() - tokens.size() / 10) {
            std::cout << "Config file seem to be binary" << std::endl;
        }
        exit(32);
    }
    ParseResult ast = Parser(tokens).parse();
    if (ast.error) {
        std::cout << ast.error << std::endl;
        delete ast.error;
        exit(33);
    }
    config result;
    Interpreter(result).visit(ast.node);
    delete ast.node;
    return result;
}

config *parse(const char *filename) {
    std::list<server>::iterator it;
    std::list<server>::iterator ite;
    config                      *result = new config;
    if (is_config_file_invalid(filename)) {
        error_exit(std::string("Invalid config file '") + filename + "'");
    }
    int fd = open(filename, O_RDONLY);
    *result = parse(fd, filename);
    it  = result->getServers().begin();
    ite = result->getServers().end();
    while (it != ite) {
        it->setConf(result);
        it++;
    }
    close(fd);
    return result;
}
