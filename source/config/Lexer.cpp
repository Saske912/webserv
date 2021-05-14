#include "Lexer.hpp"

Lexer::Lexer() : fd(0)
{
}

Lexer::Lexer(const Lexer &other) : fd(other.fd)
{
}

Lexer &Lexer::operator=(const Lexer &other)
{
	fd = other.fd;
	return *this;
}

Lexer::Lexer(int fd, const char *filename)
    : fd(fd), pos(filename),
        current_char(0), bufpos(), bufmax()
{
	advance();
}

Lexer::~Lexer()
{
}

void Lexer::read_next() {
	if (bufpos >= bufmax) {
		bufmax = read(fd, buf, 1023);
		bufpos = 0;
		if (bufmax < 1) {
			current_char = -1;
			buf[0] = 0;
			bufmax = 1;
			return;
		}
	}

	current_char = buf[bufpos++];
}

void Lexer::advance()
{
	pos.advance(current_char);
	read_next();
}

std::vector<Token> Lexer::make_tokens()
{
	std::vector<Token> tokens = std::vector<Token>();

	while (!is_eof(current_char)) {
		switch(current_char) {
			case ' ':
			case '\t':
				advance();
				break;
			case '\n':
				add_token(tokens, Token::NEWLINE);
				break;
			case ';':
				add_token(tokens, Token::SEMICOLON);
				break;
			case '{':
				add_token(tokens, Token::LCURLY);
				break;
			case '}':
				add_token(tokens, Token::RCURLY);
				break;
			case '[':
				add_token(tokens, Token::LBRACE);
				break;
			case ']':
				add_token(tokens, Token::RBRACE);
				break;
			case ',':
				add_token(tokens, Token::COMMA);
				break;
			case '#':
				add_token(tokens, Token::COMMENT);
				skip_comment();
				break;
			case '.':
			case '/':
			case '_':
			case ':':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
				tokens.push_back(make_identifier());
				break;
			default:
				add_token(tokens, Token::UNEXPECTED);
		}
	}
	tokens.push_back(Token(Token::TT_EOF, &pos));
	return tokens;
}

void Lexer::add_token(std::vector<Token> &tokens, Token::TokenType type)
{
	tokens.push_back(Token(type, &pos));
	advance();
}

Token Lexer::make_identifier()
{
	Position start_pos = pos;
	std::string identifier;

	while (is_identifier_char(current_char)) {
		identifier.push_back(current_char);
		advance();
	}

	return Token(Token::IDENTIFIER, &start_pos, &pos, identifier);
}

void Lexer::skip_comment()
{
	while (!is_eof(current_char) && current_char != '\n')
		advance();
}

bool Lexer::is_eof(char c) {
	return c == std::istream::traits_type::eof() || !c;
}

bool Lexer::is_space(char c)
{
	return c == ' ' || c == '\t';
}

bool Lexer::is_identifier_char(char c)
{
	return c == '.' || c == '/' || c == ':' || c == '_' || (c >= '0' && c <= '9') ||
		(c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
