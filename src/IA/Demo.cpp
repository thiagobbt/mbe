#include "IA/Demo.hpp"

#include "IA/EndState.hpp"

mbe::Game::State::Transition Demo::onProcessInput(Input& input) {
    auto move = players[currentPlayer].processInput(board, input);
    // Jogadas fora do tabuleiro são respondidas com -1, -1
    if (move != gm::Position{-1, -1}) {
        // Verifica se a posição está desocupada
        if (board.getPosition(move) == PlayerType::EMPTY) {
            // Marca a posição como jogada do currentPlayer
            board.play(move);

            bool jogadorGanhou = board.checkGameEnded(move);

            if (board.getNumPlays() >= 225) {
                std::cout << "End of game. Draw!" << std::endl;
                graphics.update(*this, *lastWindow);
                return {Transition::Type::REPLACE, new EndState(PlayerType::EMPTY)};
            } else if (jogadorGanhou) {
                std::cout << "End of game. Player " << currentPlayer << " won!" << std::endl;
                graphics.update(*this, *lastWindow);
                return {Transition::Type::REPLACE, new EndState(static_cast<PlayerType>(currentPlayer+1))};
            }
            // Troca o current player
            currentPlayer = 1 - currentPlayer;
        }
    }
    return {Transition::Type::SELF, this};
};
