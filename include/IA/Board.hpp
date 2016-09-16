#ifndef BOARD_HPP
#define BOARD_HPP

#include "Utilities.hpp"
#include "multimedia/Gomoku.hpp"
#include "IA/DemoGraphics.hpp"
#include <iostream>

class Board {
	friend class DemoGraphics;
 private:
 	Casa board[15][15];
 	Casa currentPlayer = Casa::BRANCA;
 	int nJogadas = 0;
 public:
 	struct Sequencias {
 		int coluna = 0;
 		int linha = 0;
 		int diag_primaria = 0;
 		int diag_secundaria = 0;
 	};

 	bool gameFinished = false;
 	gm::Position ultimaJogada;
 	Board();
 	Sequencias detectarSequencias(gm::Position pos, int tolerancia);
 	bool detectaFimDeJogo(gm::Position pos);

 	Casa getCurrentPlayer() {
 		return currentPlayer;
 	}

 	Casa getOpponent() {
 		return ((currentPlayer == Casa::BRANCA)? Casa::PRETA : Casa::BRANCA);
 	}
 	
 	void jogar(int row, int col) {
 		board[row][col] = currentPlayer;
 		nJogadas++;
 		ultimaJogada = {row, col};
 		currentPlayer = getOpponent();
 		std::cout << (int)currentPlayer << std::endl;
 	}

 	void jogar(gm::Position a) {
 		jogar(a.row, a.column);
 	}

 	Casa getPosition(int row, int col) {
 		return board[row][col];
 	}

 	gm::Position proximaVazia(gm::Position pos) {
 		int tmp = pos.column+1;
 		for (int row = pos.row; row < 15; row++) {
 			std::cout << "proximaVazia: row " << row << std::endl;
 			for (int col = tmp; col < 15; col++) {
 				std::cout << "proximaVazia: col " << col << std::endl;
 				if (getPosition(row, col) == Casa::VAZIA) {
 					std::cout << "proximaVazia: returning (" << row << ", " << col << ")" << std::endl;
 					return {row, col};
 				}
 			}
 			tmp = 0;
 		}

 		return {-1, -1};
 	}

 	int getNumJogadas() {
 		return nJogadas;
 	}
};

#endif // BOARD_HPP
