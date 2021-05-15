#include "Token.hpp"

const char *Token::tokens[] = {
    "NEWLINE", "SEMICOLON", "IDENTIFIER", "COMMENT", "LCURLY", "RCURLY",
    "LBRACE", "RBRACE", "COMMA",
    "EOF", "UNEXPECTED"
};

Token::Token() : type(Token::TT_EOF), value(), start(), end() {}

Token::Token(Token::TokenType type_, const Position *start_, const Position *end_, const std::string &value_)
    : type(type_), value(value_), start(*start_) {
    if (start_) {
        start = *start_;
        if (end_) {
            end = *end_;
        }
        else {
            end = start;
            end.advance();
        }
    }
}

Token::Token(const Token &other) : type(other.type), value(other.value), start(other.start), end(other.end) {
}

Token &Token::operator=(const Token &other) {
    type  = other.type;
    value = other.value;
    start = other.start;
    end   = other.end;
    return *this;
}

Token::~Token() {
}

std::string Token::text() const {
    switch (type) {
        case SEMICOLON:
            return ";";
        case LCURLY:
            return "{";
        case RCURLY:
            return "}";
        case LBRACE:
            return "[";
        case RBRACE:
            return "]";
        case COMMA:
            return ",";
        case TT_EOF:
        case NEWLINE:
        case COMMENT:
            return tokens[type];
        case UNEXPECTED:
        case IDENTIFIER:
            return value;
        default:
            return "<?>";
    }
}

std::ostream &operator<<(std::ostream &o, const Token &token) {
    o << Token::tokens[token.type];
    if (!token.value.empty()) {
        o << ':' << token.value;
    }
    return o;
}
