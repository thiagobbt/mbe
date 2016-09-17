#ifndef IA_CPP
#define IA_CPP

#include "IA/IA.hpp"
#include <iostream>

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

gm::Position Skynet::dummy_ia(Board& board) {
    return board.proximaVazia(board.ultimaJogada);
}

Skynet::Score Skynet::minimax(Board& board, int level, int alpha, int beta) {
    std::cout << "Skynet::minimax level: " << level << std::endl;
    // IA é preta
    Casa cor = board.getCurrentPlayer();
    std::cout << "Skynet::minimax currentPlayer: " << (int)cor << std::endl;
    int score = 0;
    Board novaBoard;
    gm::Position alphaPlay = {-1, -1};
    gm::Position betaPlay = {-1, -1};

    if (board.gameFinished) {
        std::cout << "Skynet::minimax utilidade" << std::endl;
        return {utilidade(board), board.ultimaJogada};
    } else if (level == 0) {
        std::cout << "Skynet::minimax heuristica " << board.ultimaJogada.row << " " << board.ultimaJogada.column << std::endl;
        return {heuristica(board), board.ultimaJogada};
    }

    if (cor == Casa::PRETA) {
        gm::Position child = {0, -1};
        while ((child = board.proximaVazia(child)) != (gm::Position){-1, -1}) {
            novaBoard = Board(board);
            novaBoard.jogar(child);
            score = minimax(novaBoard, level-1, alpha, beta).score;
            if (score > alpha) {
                std::cout << "Skynet::minimax " << level << " set alpha to\t" << score;
                std::cout << " on " << novaBoard.ultimaJogada.row << " " << novaBoard.ultimaJogada.column << std::endl;
                alpha = score;
                alphaPlay = novaBoard.ultimaJogada;
            }
            if (alpha >= beta) {
                std::cout << "Skynet::minimax PRETA cut" << novaBoard.ultimaJogada.row << " " << novaBoard.ultimaJogada.column << std::endl;
                return {0, novaBoard.ultimaJogada};
            }
        }
        std::cout << "Skynet::minimax returning bs " << alphaPlay.row << " " << alphaPlay.column << std::endl;
        return {alpha, alphaPlay};
    } else {
        gm::Position child = {0, -1};
        while ((child = board.proximaVazia(child)) != (gm::Position){-1, -1}) {
            novaBoard = Board(board);
            novaBoard.jogar(child);
            score = minimax(novaBoard, level-1, alpha, beta).score;
            if (score < beta) {
                std::cout << "Skynet::minimax " << level << " set beta to\t" << score;
                std::cout << " on " << novaBoard.ultimaJogada.row << " " << novaBoard.ultimaJogada.column << std::endl;
                beta = score;
                betaPlay = novaBoard.ultimaJogada;
            }
            if (alpha >= beta) {
                std::cout << "Skynet::minimax BRANCA cut" << novaBoard.ultimaJogada.row << " " << novaBoard.ultimaJogada.column << std::endl;
                return {INT_MIN, novaBoard.ultimaJogada};
            }
        }
        std::cout << "Skynet::minimax returning bs " << betaPlay.row << " " << alphaPlay.column << std::endl;
        return {beta, betaPlay};
    }
    std::cout << "Skynet::minimax returning ultra bs " << betaPlay.row << " " << alphaPlay.column << std::endl;
    return {-1, novaBoard.ultimaJogada};
}

gm::Position Skynet::minimax_base(Board& board) {
    std::cout << "Skynet::minimax_base" << std::endl;
    gm::Position pos = minimax(board, 3).position;
    std::cout << "MINIMAX RETURNED POS(" << pos.row << ", " << pos.column << ")" << std::endl;
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
    std::cout << "Skynet::heuristica" << std::endl;
    Board::Sequencias sequencias_f = board.detectarSequencias(board.ultimaJogada, 0);
    Board::Sequencias sequencias_a = board.detectarSequencias(board.ultimaJogada, 1);

    int score = 0;

    score = avaliar(sequencias_f.coluna) + avaliar(sequencias_f.linha) + avaliar(sequencias_f.diag_primaria) + avaliar(sequencias_f.diag_secundaria);
    score *= multiplicadorScore[board.ultimaJogada.row][board.ultimaJogada.column];
    auto lp = board.ultimaJogada;
    std::cout << "heuristica(" << lp.row << ", " << lp.column << ") = " << score << std::endl;

    return score;
}

int Skynet::utilidade(Board& board) {
    std::cout << "Skynet::utilidade" << std::endl;
    return 10000/(board.getNumJogadas()-4);
}

gm::Position Skynet::doUpdate(Board& board, gm::GameInput& input) {
    return minimax_base(board);
}

void Skynet::setCor(Casa c) {
    cor = c;
}

#endif // IA_CPP
