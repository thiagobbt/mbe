#ifndef IA_CPP
#define IA_CPP

#ifdef RELEASE
    #define DBOUT( x )
#else
    #include <iostream>
    #define DBOUT( x )  std::cout << x << std::endl
#endif

#include "IA/IA.hpp"

Skynet::Skynet() {
    int linha1[15] = {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1};
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 15; j++) {
            multiplicadorScore[i][j] = linha1[j];
            multiplicadorScore[i+4][j] = linha1[j]*2;
            multiplicadorScore[i+7][j] = linha1[j]*2;
            multiplicadorScore[i+11][j] = linha1[j];
        }
    }

    for (int i = 6; i < 9; i++) {
        for (int j = 6; j < 9; j++) {
            multiplicadorScore[i][j] = 8;
        }
    }
}

Skynet::Score Skynet::minimax(Board& board, int level, int alpha, int beta) {
    DBOUT("Skynet::minimax level: " << level);
    // IA é preta
    Casa cor = board.getCurrentPlayer();
    DBOUT("Skynet::minimax currentPlayer: " << (int)cor);
    int score = 0;
    Board novaBoard;
    gm::Position alphaPlay = {-1, -1};
    gm::Position betaPlay = {-1, -1};

    if (board.gameFinished) {
        DBOUT("Skynet::minimax utilidade");
        return {utilidade(board), board.ultimaJogada};
    } else if (level == 0) {
        DBOUT("Skynet::minimax heuristica " << board.ultimaJogada.row << " " << board.ultimaJogada.column);
        return {heuristica(board), board.ultimaJogada};
    }

    std::list<gm::Position> children = board.getChildren();

    if (cor == Casa::BRANCA) {
        for (gm::Position child : children) {
            novaBoard = Board(board);
            novaBoard.jogar(child);
            score = minimax(novaBoard, level-1, alpha, beta).score;
            if (score > alpha) {
                DBOUT("Skynet::minimax " << level << " set alpha to\t" << score
                       << " on " << novaBoard.ultimaJogada.row << " " << novaBoard.ultimaJogada.column);
                alpha = score;
                alphaPlay = novaBoard.ultimaJogada;
            }
            /*if (alpha >= beta) {
                DBOUT("Skynet::minimax PRETA cut " << novaBoard.ultimaJogada.row << " " << novaBoard.ultimaJogada.column);
                return {0, novaBoard.ultimaJogada};
            }*/
        }
        DBOUT("Skynet::minimax returning bs " << alphaPlay.row << " " << alphaPlay.column);
        return {alpha, alphaPlay};
    } else {
        for (gm::Position child : children) {
            novaBoard = Board(board);
            novaBoard.jogar(child);
            score = minimax(novaBoard, level-1, alpha, beta).score;
            if (score < beta) {
                DBOUT("Skynet::minimax " << level << " set beta to\t" << score
                       << " on " << novaBoard.ultimaJogada.row << " " << novaBoard.ultimaJogada.column);
                beta = score;
                betaPlay = novaBoard.ultimaJogada;
            }
            /*if (alpha >= beta) {
                DBOUT("Skynet::minimax BRANCA cut " << novaBoard.ultimaJogada.row << " " << novaBoard.ultimaJogada.column);
                return {INT_MIN, novaBoard.ultimaJogada};
            }*/
        }
        DBOUT("Skynet::minimax returning bs " << betaPlay.row << " " << alphaPlay.column);
        return {beta, betaPlay};
    }
    DBOUT("Skynet::minimax returning ultra bs " << betaPlay.row << " " << alphaPlay.column);
    return {-1, novaBoard.ultimaJogada};
}

gm::Position Skynet::minimax_base(Board& board) {
    DBOUT("Skynet::minimax_base");
    gm::Position pos = minimax(board, 2).position;
    DBOUT("MINIMAX RETURNED POS(" << pos.row << ", " << pos.column << ")");
    return pos;
}

int avaliar(int a) {
    switch (a) {
        case 0: return 0;
        case 1: return 1;
        case 2: return 10;
        case 3: return 50;
        case 4: return 1000;
        default: return 194584;
    }
}

int Skynet::heuristica(Board& board) {
    DBOUT("Skynet::heuristica");
    Board::Sequencias sequencias_f = board.detectarSequencias(board.ultimaJogada, 0);
    Board::Sequencias sequencias_a = board.detectarSequencias(board.ultimaJogada, 1);

    int score = 0;

    score = avaliar(sequencias_f.coluna) + avaliar(sequencias_f.linha) + avaliar(sequencias_f.diag_primaria) + avaliar(sequencias_f.diag_secundaria);
    score *= multiplicadorScore[board.ultimaJogada.row][board.ultimaJogada.column];
    auto lp = board.ultimaJogada;
    DBOUT("heuristica(" << lp.row << ", " << lp.column << ") = " << score);

    return score;
}

int Skynet::utilidade(Board& board) {
    DBOUT("Skynet::utilidade");
    return 10000/(board.getNumJogadas()-4);
}

gm::Position Skynet::doUpdate(Board& board, gm::GameInput& input) {
    return minimax_base(board);
}

void Skynet::setCor(Casa c) {
    cor = c;
}

#endif // IA_CPP
