#include <iostream>
#include "mbe/Engine.hpp"
#include "multimedia/Gomoku.hpp"
#include "IA/Utilities.hpp"
#include "IA/IA.hpp"
#include "IA/DemoPlayer.hpp"
#include "IA/DemoGraphics.hpp"
#include "IA/EndState.hpp"
#include "IA/Demo.hpp"

int main(int argc, char** argv) {
    auto *ia = new Skynet();
    ia->setCor(Casa::PRETA);
    auto *gameState = new Demo(DemoPlayer(), DemoPlayer(ia));
    auto *game = new mbe::Game(gameState);

    mbe::Engine e(game, 1024, 768, "Demo");
    e.run();

    return 0;
}
