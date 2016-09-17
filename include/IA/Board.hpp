#ifndef BOARD_HPP
#define BOARD_HPP

#include "Utilities.hpp"
#include "multimedia/Gomoku.hpp"
#include "IA/DemoGraphics.hpp"

#ifdef RELEASE
    #define DBOUT( x )
#else
    #include <iostream>
    #define DBOUT( x )  std::cout << x << std::endl
#endif

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
    	DBOUT("Board::jogar(" << row << ", " << col << ")");
        board[row][col] = currentPlayer;
        nJogadas++;
        ultimaJogada = {row, col};
        currentPlayer = getOpponent();
    }

    void jogar(gm::Position a) {
        jogar(a.row, a.column);
    }

    Casa getPosition(int row, int col) {
        return board[row][col];
    }

    std::list<gm::Position> getChildren() {
    	std::list<gm::Position> children;
    	for (int i = 0; i < 225; i++) {
    		int row = i / 15;
    		int col = i % 15;
    		if (getPosition(row, col) == Casa::VAZIA) {
    			children.push_back({row, col});
            }
    	}
    	return children;
    }

    int getNumJogadas() {
        return nJogadas;
    }
};

#endif // BOARD_HPP
