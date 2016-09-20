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
	EMPTY,
	WHITE,
	BLACK
};

inline std::ostream& operator<<(std::ostream& stream, const PlayerType& p) {
	switch (p) {
		case PlayerType::WHITE:
			stream << "W";
			break;
		case PlayerType::BLACK:
			stream << "B";
			break;
		default:
			stream << ".";
			break;
	}
	return stream;
};

#endif // UTILITIES_HPP
