#ifndef POSITION_HPP
#define POSITION_HPP

class Position {
public:
    Position();

    Position(const Position &other);

    Position(const char *filename);

    Position &operator=(const Position &other);

    ~Position();

    void advance(char c = 0);

    int        line;
    int        col;
    const char *filename;
};

#endif
