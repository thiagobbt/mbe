#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#ifdef RELEASE
    #define DBOUT( x )
#else
    #include <iostream>
    #define DBOUT( x )  std::cout << x << std::endl
#endif

#include <ostream>

enum class PlayerType {
    VAZIA,
    BRANCA,
    PRETA
};

enum class Direction {
    NW, N, NE,
    W,     E,
    SW, S, SE
};

inline std::ostream& operator<<(std::ostream& stream, const PlayerType& p) {
	switch (p) {
		case PlayerType::BRANCA:
			stream << "B";
			break;
		case PlayerType::PRETA:
			stream << "P";
			break;
		default:
			stream << "V";
			break;
	}
	return stream;
};

// using Board = PlayerType[15][15];

#endif // UTILITIES_HPP
