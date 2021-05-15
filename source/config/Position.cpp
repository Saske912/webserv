#include "Position.hpp"

Position::Position() : line(1), col(0), filename("<...>") {
}

Position::Position(const Position &other)
    : line(other.line), col(other.col), filename(other.filename) {
}

Position::Position(const char *filename)
    : line(1), col(0), filename(filename) {
}

Position &Position::operator=(const Position &other) {
    line     = other.line;
    col      = other.col;
    filename = other.filename;
    return *this;
}

Position::~Position() {
}

void Position::advance(char c) {
    ++col;
    if (c == '\n') {
        ++line;
        col = 1;
    }
}
