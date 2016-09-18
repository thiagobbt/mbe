#ifndef IA_HPP
#define IA_HPP

#include "Utilities.hpp"
#include "multimedia/Gomoku.hpp"
#include "IA/Board.hpp"
#include <climits>

class Skynet : public gm::PlayerHandler<Board> {
 private:
    PlayerType cor;

    struct Score {
        int score;
        gm::Position position;
    };

    Score minimax(Board& board, int level, int alpha = INT_MIN, int beta = INT_MAX);
    gm::Position minimax_base(Board& board);
    gm::Position dummy_ia(Board& board);

    int heuristica(Board& board);

    int utilidade(Board& board);
    gm::Position doUpdate(Board& board, gm::GameInput& input) override;

    int multiplicadorScore[15][15];

 public:
    Skynet();
    void setCor(PlayerType c);
};

#endif // IA_HPP
