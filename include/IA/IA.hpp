#ifndef IA_HPP
#define IA_HPP

#include "IA/Utilities.hpp"
#include "multimedia/Gomoku.hpp"
#include "IA/Board.hpp"
#include <climits>

class Skynet : public gm::PlayerHandler<Board> {
 private:
    int utility(PlayerType player, Board& board);
    int avaliar(int a);
    int heuristic(PlayerType player, Board& board);
    int minimax(PlayerType player, Board& board, int level = 0, int alpha = INT_MIN, int beta = INT_MAX);
    gm::Position minimax_base(Board& board);
    gm::Position doUpdate(Board& board, gm::GameInput& input) override;

    int scoreMultiplier[15][15];
    int iterations = 0;
};

#endif // IA_HPP
