#ifndef POSITION_HPP
#define POSITION_HPP

class Position
{
public:
	Position();
	Position(const Position& other);
	Position(int idx, int line, int col);
	Position& operator=(const Position& other);
	~Position();

	void advance(char c = 0);

	int idx;
	int line;
	int col;
};

#endif
