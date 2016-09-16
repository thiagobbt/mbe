#ifndef IA_HPP
#define IA_HPP

#include "Utilities.hpp"
#include "multimedia/Gomoku.hpp"
#include "IA/Board.hpp"
#include <climits>

class Skynet : public gm::PlayerHandler<Board> {
 private:
    Casa cor;

    int minimax(Board& board, int level, Casa cor, int alpha = INT_MAX, int beta = INT_MAX);
    gm::Position minimax_base(Board& board);
    gm::Position dummy_ia(Board& board);

    int heuristica(Board& board, int row, int col);

    int utilidade(Board& board);
    gm::Position doUpdate(Board& board, gm::GameInput& input) override;

 public:
    void setCor(Casa c);
};

#endif // IA_HPP
