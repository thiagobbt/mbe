#ifndef BOARD_HPP
#define BOARD_HPP

#include "Utilities.hpp"
#include "multimedia/Gomoku.hpp"
#include "IA/DemoGraphics.hpp"

#include <unordered_set>

class Board {
private:
    PlayerType board[15][15];
    PlayerType currentPlayer = PlayerType::WHITE;
    uint numPlays = 0;
    bool gameEnded = false;
    gm::Position lastPlay = {-1, -1};
    std::list<gm::Position> plays;

public:
    struct Sequencias {
        int coluna = 0;
        int linha = 0;
        int diag_primaria = 0;
        int diag_secundaria = 0;

        Sequencias(int c, int l, int d1, int d2)
        : coluna(c), linha(l), diag_primaria(d1), diag_secundaria(d2) {}
    };

    PlayerType getPosition(gm::Position pos);
    PlayerType getOpponent();
    PlayerType getCurrentPlayer();
    gm::Position getLastPlay();
    uint getNumPlays();
    std::unordered_set<gm::Position> getChildren();
    bool isGameEnded();
    Sequencias detectarSequencias(gm::Position pos, int tolerancia);
    bool checkGameEnded(gm::Position pos);
    bool play(gm::Position pos, bool debug = true);

    friend std::ostream& operator<<(std::ostream& stream, const Board& b) {
        stream << "gameEnded: " << b.gameEnded << std::endl;
        stream << std::hex;
        for (int i = 0; i < 15; ++i) {
            if (i == 0) {
                for (int j = 0; j < 15; ++j) {
                    if (j == 0) stream << "  ";
                    stream << j << ' ';
                }
                stream << std::endl;
            }
            for (int j = 0; j < 15; ++j) {
                if (j == 0) stream << i << " ";
                stream << b.board[i][j] << ' ';
            }
            if (i < 14) stream << std::endl;
        }
        stream << std::dec;
        return stream;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Sequencias& s) {
        stream << "co: " << s.coluna << std::endl;
        stream << "li: " << s.linha << std::endl;
        stream << "d1: " << s.diag_primaria << std::endl;
        stream << "d2: " << s.diag_secundaria << std::endl;
        return stream;
    }
};

#endif // BOARD_HPP
