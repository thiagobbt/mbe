#ifndef BOARD_HPP
#define BOARD_HPP

#include "Utilities.hpp"
#include "multimedia/Gomoku.hpp"
#include "IA/DemoGraphics.hpp"

class Board {
    friend class DemoGraphics;
 private:
    PlayerType board[15][15];
    PlayerType currentPlayer = PlayerType::BRANCA;
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

    PlayerType getCurrentPlayer() {
        return currentPlayer;
    }

    PlayerType getOpponent() {
        return ((currentPlayer == PlayerType::BRANCA)? PlayerType::PRETA : PlayerType::BRANCA);
    }
    
    void jogar(int row, int col) {
    	DBOUT("Board::jogar(" << row << ", " << col << ")");
        board[row][col] = currentPlayer;
        nJogadas++;
        ultimaJogada = {row, col};
        currentPlayer = getOpponent();
        DBOUT(*this);
    }

    void jogar(gm::Position a) {
        jogar(a.row, a.column);
    }

    PlayerType getPosition(int row, int col) {
        return board[row][col];
    }

    std::list<gm::Position> getChildren() {
    	std::list<gm::Position> children;
    	for (int i = 0; i < 225; i++) {
    		int row = i / 15;
    		int col = i % 15;
    		if (getPosition(row, col) == PlayerType::VAZIA) {
    			children.push_back({row, col});
            }
    	}
    	return children;
    }

    int getNumJogadas() {
        return nJogadas;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Board& b) {
        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                stream << b.board[i][j] << ' ';
            }
            if (i < 14) stream << std::endl;
        }
        return stream;
    }
};

#endif // BOARD_HPP
