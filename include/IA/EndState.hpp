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
    EndState(PlayerType cor = PlayerType::PRETA) : corGanhador(cor){}

 private:
    PlayerType corGanhador;

    void onPeriodicUpdate() override {}

    void onUpdateRenderer(Renderer& window) override {
        std::string texto;

        if (corGanhador == PlayerType::BRANCA) {
            texto = "Branco ganhou!";
        } else if (corGanhador == PlayerType::PRETA) {
            texto = "Preto ganhou!";
        } else {
            texto = "Empate!";
        }

        // Declare and load a font
        sf::Font font;
        font.loadFromFile("arial.ttf");
        // Create a text
        sf::Text text(texto, font);
        text.setCharacterSize(30);
        text.setStyle(sf::Text::Bold);
        //text.setOutlineColor(sf::Color::White);
#if SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 4)
        text.setFillColor(sf::Color::White);
#else
        text.setColor(sf::Color::White);
#endif
        //text.setOutlineThickness(1.0f);
        //text.setPosition(300, 200);
        auto position = window.getSize();
        text.setPosition(sf::Vector2f(position.x/2 - text.getGlobalBounds().width / 2, position.y/2));
        window.clear(sf::Color::Black);

        // Desenha
        window.draw(text);
    }

    Transition onProcessInput(Input& input) override;
};

#endif /* ENDSTATE_HPP */
