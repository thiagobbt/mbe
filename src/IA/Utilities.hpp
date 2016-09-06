#ifndef UTILITIES_HPP
#define UTILITIES_HPP

enum class Casa {
    VAZIA,
    BRANCA,
    PRETA
};

enum class Direction {
    NW, N, NE,
    W,     E,
    SW, S, SE
};

using Board = Casa[15][15];

#endif // UTILITIES_HPP
