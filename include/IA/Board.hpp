#ifndef BOARD_HPP
#define BOARD_HPP

#include "Utilities.hpp"
#include "multimedia/Gomoku.hpp"
#include "IA/DemoGraphics.hpp"

class Board {
	friend class DemoGraphics;
 private:
 	Casa board[15][15];
 public:
 	int nJogadas = 0;
 	bool gameFinished = false;
 	gm::Position ultimaJogada;
 	Board();
 	int* detectarSequencias(int row, int col, Casa cor, int tolerancia);
 	bool detectaFimDeJogo(int row, int col, Casa cor);
 	
 	void jogar(Casa cor, int row, int col) {
 		board[row][col] = cor;
 		nJogadas++;
 		ultimaJogada = {row, col};
 	}

 	void jogar(Casa cor, gm::Position a) {
 		jogar(cor, a.row, a.column);
 	}

 	Casa getPosition(int row, int col) {
 		return board[row][col];
 	}

 	gm::Position proximaVazia() {
 		int x = 0;
	    for (auto &row : board) {
	        int y = 0;
	        for (auto place : row) {
	            if (place == Casa::VAZIA) {
	                return {x, y};
	            }
	            y++;
	        }
	        x++;
	    }
	    return {-1, -1};
 	}
};

#endif // BOARD_HPP
