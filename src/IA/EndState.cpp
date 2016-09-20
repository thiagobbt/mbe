#include "IA/EndState.hpp"
#include "IA/Demo.hpp"

mbe::Game::State::Transition EndState::onProcessInput(Input& input) {
    for (auto event : input) {
        if (event.type == sf::Event::MouseButtonPressed) {
            //return {Transition::Type::REPLACE, new Demo(DemoPlayer(), DemoPlayer(new Skynet()))};
            return {Transition::Type::RESTORE, nullptr};
        }
    }
    return {Transition::Type::SELF, this};
}
