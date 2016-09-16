#ifndef IA_CPP
#define IA_CPP

#include "IA/IA.hpp"

gm::Position Skynet::dummy_ia(Board& board) {
    return board.proximaVazia();
}

Casa getOpponent(Casa cor) {
	return cor == Casa::BRANCA? Casa::PRETA : Casa::BRANCA;
}

int Skynet::minimax(Board& board, int level, Casa cor, int alpha, int beta) {
	// TODO: Implement this

	// IA é preta


	int score = 0;

	if (board.gameFinished || level == 0) {
		return utilidade(board);
	}

	if (cor == Casa::PRETA) {
		gm::Position child;
		while ((child = board.proximaVazia()) != (gm::Position){-1, -1}) {
			Board novaBoard = Board(board);
        	novaBoard.jogar(cor, child);
            score = minimax(novaBoard, level-1, getOpponent(cor), alpha, beta);
            if (score > alpha) alpha = score;
            if (alpha >= beta) return alpha;
		}
	    return alpha;
	} else {
		gm::Position child;
		while ((child = board.proximaVazia()) != (gm::Position){-1, -1}) {
        	Board novaBoard = Board(board);
        	novaBoard.jogar(cor, child);
            score = minimax(novaBoard, level-1, getOpponent(cor), alpha, beta);
            if (score < beta) beta = score;
            if (alpha >= beta) return beta;
	    }
	    return beta;
	}

    return -1;
}

gm::Position Skynet::minimax_base(Board& board) {
	// TODO: Implement this
	return dummy_ia(board);
	return {-1, -1};
}

int Skynet::heuristica(Board& board, int row, int col) {
    int* sequencias_f = board.detectarSequencias(row, col, cor, 0);
    int* sequencias_a = board.detectarSequencias(row, col, cor, 1);

    return 42;
}

int Skynet::utilidade(Board& board) {
    return 10000/(board.nJogadas-4);
}

gm::Position Skynet::doUpdate(Board& board, gm::GameInput& input) {
    return minimax_base(board);
}

void Skynet::setCor(Casa c) {
    cor = c;
}

#endif // IA_CPP
