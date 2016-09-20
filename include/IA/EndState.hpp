#ifndef ENDSTATE_HPP
#define ENDSTATE_HPP

#include <iostream>
#include "mbe/Engine.hpp"
#include "base/GameState.hpp"
#include "IA/Utilities.hpp"
#include "IA/IA.hpp"

class Demo;

class EndState : public mbe::Game::State {
 public:
    EndState(PlayerType cor = PlayerType::BLACK) : corGanhador(cor){}

 private:
    PlayerType corGanhador;

    void onPeriodicUpdate() override {}

    void onUpdateRenderer(Renderer& window) override {
        std::string texto;

        if (corGanhador == PlayerType::WHITE) {
            texto = "White wins!";
        } else if (corGanhador == PlayerType::BLACK) {
            texto = "Black wins!";
        } else {
            texto = "Draw!";
        }

        // Declare and load a font
        sf::Font font;
        font.loadFromFile("visitor1.ttf");
        // Create a text
        sf::Text text(texto, font);
        text.setCharacterSize(40);
        //text.setStyle(sf::Text::Bold);
        //text.setOutlineColor(sf::Color::White);
#if SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 4)
        text.setFillColor(sf::Color::White);
#else
        text.setColor(sf::Color::White);
#endif
        auto position = window.getSize();
        //text.setPosition(sf::Vector2f(position.x/2 - text.getGlobalBounds().width / 2, position.y/2));

        text.setPosition(sf::Vector2f(position.x - text.getGlobalBounds().width - 30, position.y/2));
        //window.clear(sf::Color::Black);

        // Desenha
        window.draw(text);
    }

    Transition onProcessInput(Input& input) override;
};

#endif /* ENDSTATE_HPP */
