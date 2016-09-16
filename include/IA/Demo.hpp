#ifndef DEMO_HPP
#define DEMO_HPP

#include "IA/DemoPlayer.hpp"
#include "IA/EndState.hpp"
#include "IA/DemoGraphics.hpp"
#include "IA/Board.hpp"
#include <algorithm>


class Demo : public mbe::Game::State {
 public:
    Demo(DemoPlayer&& p1 = DemoPlayer(),
         DemoPlayer&& p2 = DemoPlayer()) 
    : players{std::move(p1), std::move(p2)} {}

    const Board& getBoard() const { return board; }

 private:
    DemoGraphics graphics;
    std::array<DemoPlayer, 2> players;
    short currentPlayer = 0;
    Board board;

    int* getPos(int row, int col, Direction d) {
        switch (d) {
            case Direction::NW:
                return new int[2]{row-1, col-1};
            case Direction::N:
                return new int[2]{row-1, col};
            case Direction::NE:
                return new int[2]{row-1, col+1};
            case Direction::W:
                return new int[2]{row, col-1};
            case Direction::E:
                return new int[2]{row, col+1};
            case Direction::SW:
                return new int[2]{row+1, col-1};
            case Direction::S:
                return new int[2]{row+1, col};
            case Direction::SE:
                return new int[2]{row+1, col+1};
        }
    }

    // Agora nosso onUpdateRenderer chama o update de graphics
    void onUpdateRenderer(Renderer& window) override {
        graphics.update(*this, window);
    }

    // Aqui, mandamos o input para player e recebemos uma gm::Position de
    // resposta. Ela representa a jogada do player.
    Transition onProcessInput(Input& input) override {
        auto move = players[currentPlayer].processInput(board, input);
        // Jogadas fora do tabuleiro são respondidas com -1, -1
        if (move != gm::Position{-1, -1}) {
            // Verifica se a posição está desocupada
            if (board.getPosition(move.row, move.column) == Casa::VAZIA) {
                // Marca a posição como jogada do currentPlayer
                board.jogar(move);

                bool jogadorGanhou = board.detectaFimDeJogo(move);

                if (jogadorGanhou) {   
                    std::cout << "Fim de Jogo. Jogador " << currentPlayer << " venceu!" << std::endl;
                    return {Transition::Type::STORE, new EndState(static_cast<Casa>(currentPlayer+1))};
                } else if (board.getNumJogadas() >= 225) {
                    std::cout << "Fim de Jogo. Empate!" << std::endl;
                    return {Transition::Type::STORE, new EndState(Casa::VAZIA)};
                }

                // Troca o current player
                currentPlayer = 1 - currentPlayer;
            }
        }
        return {Transition::Type::SELF, this};
    }
};

#endif /* DEMO_HPP */

