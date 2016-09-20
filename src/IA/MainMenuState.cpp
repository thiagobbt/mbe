#include "IA/MainMenuState.hpp"
#include "IA/DemoPlayer.hpp"
#include "IA/Demo.hpp"
#include "IA/IA.hpp"

#include <iostream>

mbe::Game::State::Transition MainMenuState::onProcessInput(Input& input) {
    for (auto event : input) {
        if (event.type == sf::Event::MouseButtonPressed) {
            int x = event.mouseButton.x;
            int y = event.mouseButton.y;
            if (x >= 340 && x <= 650) {
                if (y >= 250 && y <= 290) {
                    return {Transition::Type::STORE, new Demo(DemoPlayer(), DemoPlayer(new Skynet()))};
                } else if (y >= 300 && y <= 330) {
                    return {Transition::Type::STORE, new Demo(DemoPlayer(new Skynet()), DemoPlayer())};
                } else if (y >= 340 && y <= 380) {
                    return {Transition::Type::STORE, new Demo(DemoPlayer(), DemoPlayer())};    
                } else if (y >= 390 && y <= 420) {
                    return {Transition::Type::STORE, new Demo(DemoPlayer(new Skynet()), DemoPlayer(new Skynet()))};
                }
            }

            //return {Transition::Type::STORE, new Demo(DemoPlayer(), DemoPlayer(new Skynet()))};
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Num1) 
                return {Transition::Type::STORE, new Demo(DemoPlayer(), DemoPlayer(new Skynet()))};
            else if (event.key.code == sf::Keyboard::Num2) 
                return {Transition::Type::STORE, new Demo(DemoPlayer(new Skynet()), DemoPlayer())};
            else if (event.key.code == sf::Keyboard::Num3) 
                return {Transition::Type::STORE, new Demo(DemoPlayer(), DemoPlayer())};
            else if (event.key.code == sf::Keyboard::Num4) 
                return {Transition::Type::STORE, new Demo(DemoPlayer(new Skynet()), DemoPlayer(new Skynet()))};
        }
    }
    return {Transition::Type::SELF, this};
}
