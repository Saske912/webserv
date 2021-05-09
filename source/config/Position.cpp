#include "Position.hpp"

Position::Position() : idx(), line(1), col()
{
}

Position::Position(const Position &other) : idx(other.idx), line(other.line), col(other.col)
{
}

Position::Position(int idx, int line, int col) : idx(idx), line(line), col(col)
{
}

Position &Position::operator=(const Position &other)
{
	idx = other.idx;
	line = other.line;
	col = other.col;
	return *this;
}

Position::~Position()
{
}

void Position::advance(char c)
{
	++idx;
	++col;
	if (c == '\n') {
		++line;
		col = 0;
	}
}
