#ifndef DEMO_HPP
#define DEMO_HPP

#include "IA/DemoPlayer.hpp"
#include "IA/DemoGraphics.hpp"
#include "IA/Board.hpp"
#include "mbe/Engine.hpp"
#include "base/GameState.hpp"
#include <algorithm>


class Demo : public mbe::Game::State {
 public:
    Demo(DemoPlayer&& p1 = DemoPlayer(),
         DemoPlayer&& p2 = DemoPlayer()) 
    : players{std::move(p1), std::move(p2)} {
        board = Board();
    }

    const Board& getBoard() const { return board; }

 private:
    DemoGraphics graphics;
    std::array<DemoPlayer, 2> players;
    short currentPlayer = 0;
    Board board;

    // Agora nosso onUpdateRenderer chama o update de graphics
    void onUpdateRenderer(Renderer& window) override {
        graphics.update(*this, window);
    }

    // Aqui, mandamos o input para player e recebemos uma gm::Position de
    // resposta. Ela representa a jogada do player.
    Transition onProcessInput(Input& input) override;
};

#endif /* DEMO_HPP */
