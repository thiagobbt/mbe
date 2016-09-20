#ifndef IA_CPP
#define IA_CPP

#include "IA/IA.hpp"

int Skynet::utility(PlayerType player, Board& board) {
    if (!board.isGameEnded()) {
        // DB("IA::utility error game not ended");
        return 0;
    }
    int score = 100000/(board.getNumPlays()-4);
    PlayerType winner = board.getOpponent();
    if (player != winner) score *= -1;
    return score;
}

int Skynet::avaliar(int a) {
    switch (a) {
        case 0: return 0;
        case 1: return 1;
        case 2: return 10;
        case 3: return 50;
        case 4: return 1000;
        default: return 10000;
    }
}

int Skynet::heuristic(PlayerType player, Board& board) {
    int score = 0, score1 = 0, score2 = 0;

    gm::Position lastPlay = board.getLastPlay();

    Board::Sequencias seq = board.detectarSequencias(lastPlay, 0);
    score1 += avaliar(seq.coluna); 
    score1 += avaliar(seq.linha);
    score1 += avaliar(seq.diag_primaria);
    score1 += avaliar(seq.diag_secundaria);

    seq = board.detectarSequencias(lastPlay, 1);
    score2 += avaliar(seq.coluna); 
    score2 += avaliar(seq.linha);
    score2 += avaliar(seq.diag_primaria);
    score2 += avaliar(seq.diag_secundaria);

    score = score1 + score2;

    // score *= Skynet::scoreMultiplier[lastPlay.row][lastPlay.column];

    if (player != board.getOpponent()) {
        score *= -1.2;
    }
    return score;
};

int Skynet::minimax(PlayerType player, Board& board, int level, int alpha, int beta) {
    if (board.checkGameEnded(board.getLastPlay())) {
        return utility(player, board);
    }

    if (level == 0) {
        return heuristic(player, board);
    }

    std::list<gm::Position> children = board.getChildren();

    if (board.getCurrentPlayer() == player) {
        // MAX
        // DBOUT("IA::minimax MAX");
        for (gm::Position child : children) {
            Board childBoard = Board(board);
            childBoard.play(child, false);
            int score = minimax(player, childBoard, level-1, alpha, beta);
            if (score > alpha) alpha = score;
            if (alpha >= beta) return alpha;
        }
        return alpha;
    } else {
        // MIN
        // DBOUT("IA::minimax MIN");
        for (gm::Position child : children) {
            Board childBoard = Board(board);
            childBoard.play(child, false);
            int score = minimax(player, childBoard, level-1, alpha, beta);
            if (score < beta) beta = score;
            if (alpha >= beta) return beta;
        }
        return beta;
    }

    // DBOUT("IA::minimax error");
    return 0;
}

gm::Position Skynet::minimax_base(Board& board) {

    PlayerType player = board.getCurrentPlayer();

    std::list<gm::Position> children = board.getChildren();
    int bestScore = INT_MIN;
    gm::Position bestPosition = {-1, -1};

        for (auto child = children.begin(); child != children.end(); child++) {
            Board childBoard = Board(board);
            childBoard.play(*child);
            int score = minimax(player, childBoard, 3, bestScore);
            if (score > bestScore) {
                bestScore = score;
                bestPosition = *child;
            }
        }

    return bestPosition;
}

gm::Position Skynet::doUpdate(Board& board, gm::GameInput& input) {
    return minimax_base(board);
}

#endif // IA_CPP
