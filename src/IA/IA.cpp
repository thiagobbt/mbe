#ifndef IA_CPP
#define IA_CPP

#include "IA/IA.hpp"
#include <iostream>

gm::Position Skynet::dummy_ia(Board& board) {
    return board.proximaVazia(board.ultimaJogada);
}

Skynet::Score Skynet::minimax(Board& board, int level, int alpha, int beta) {
    std::cout << "Skynet::minimax" << std::endl;
    // IA é preta
    Casa cor = board.getCurrentPlayer();
    std::cout << "currentPlayer = " << (int)cor << std::endl;
    int score = 0;
    Board novaBoard;
    gm::Position alphaPlay;
    gm::Position betaPlay;

    if (board.gameFinished) {
        std::cout << "utilidade" << std::endl;
        return {utilidade(board), board.ultimaJogada};
    } else if (level == 0) {
        std::cout << "heuristica " << board.ultimaJogada.row << " " << board.ultimaJogada.column << std::endl;
        return {heuristica(board), board.ultimaJogada};
    }

    if (cor == Casa::PRETA) {
        gm::Position child = {0, -1};
        while ((child = board.proximaVazia(child)) != (gm::Position){-1, -1}) {
            novaBoard = Board(board);
            novaBoard.jogar(child);
            score = minimax(novaBoard, level-1, alpha, beta).score;
            if (score > alpha) {
                alpha = score;
                alphaPlay = novaBoard.ultimaJogada;
            }
            if (alpha >= beta) {
                std::cout << "cut" << std::endl;
                return {INT_MIN, novaBoard.ultimaJogada};
            }
        }
        return {alpha, alphaPlay};
    } else {
        gm::Position child;
        while ((child = board.proximaVazia(child)) != (gm::Position){-1, -1}) {
            novaBoard = Board(board);
            novaBoard.jogar(child);
            score = minimax(novaBoard, level-1, alpha, beta).score;
            if (score < beta) {
                beta = score;
                betaPlay = novaBoard.ultimaJogada;
            }
            if (alpha >= beta) {
                std::cout << "cut" << std::endl;
                return {INT_MAX, novaBoard.ultimaJogada};
            }
        }
        return {beta, betaPlay};
    }
    return {-1, novaBoard.ultimaJogada};
}

gm::Position Skynet::minimax_base(Board& board) {
    std::cout << "Skynet::minimax_base" << std::endl;
    gm::Position pos = minimax(board, 1).position;
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
