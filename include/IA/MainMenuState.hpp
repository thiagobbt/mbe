#ifndef ENDSTATE_HPP
#define ENDSTATE_HPP

#include <iostream>
#include "mbe/Engine.hpp"
#include "base/GameState.hpp"

class Demo;

class MainMenuState : public mbe::Game::State {
 private:
    void onPeriodicUpdate() override {}

    void onUpdateRenderer(Renderer& window) override {
        std::string title_str = "Select an option: ";
        std::string mode1_str = "[1] Player vs AI";
        std::string mode2_str = "[2] AI vs Player";
        std::string mode3_str = "[3] Player vs Player";
        std::string mode4_str = "[4] AI vs AI";
        std::string mode5_str = "[Q] Exit";

        // Declare and load a font
        sf::Font font;
        font.loadFromFile("visitor1.ttf");

        // Create text
        sf::Text title(title_str, font);
        sf::Text mode1(mode1_str, font);
        sf::Text mode2(mode2_str, font);
        sf::Text mode3(mode3_str, font);
        sf::Text mode4(mode4_str, font);
        sf::Text mode5(mode5_str, font);


        title.setCharacterSize(30);
        mode1.setCharacterSize(30);
        mode2.setCharacterSize(30);
        mode3.setCharacterSize(30);
        mode4.setCharacterSize(30);
        mode5.setCharacterSize(30);

        title.setStyle(sf::Text::Bold);
        mode1.setStyle(sf::Text::Bold);
        mode2.setStyle(sf::Text::Bold);
        mode3.setStyle(sf::Text::Bold);
        mode4.setStyle(sf::Text::Bold);
        mode5.setStyle(sf::Text::Bold);

#if SFML_VERSION_MAJOR > 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR >= 4)
        title.setFillColor(sf::Color::White);
        mode1.setFillColor(sf::Color::White);
        mode2.setFillColor(sf::Color::White);
        mode3.setFillColor(sf::Color::White);
        mode4.setFillColor(sf::Color::White);
        mode5.setFillColor(sf::Color::White);
#else
        title.setColor(sf::Color::White);
        mode1.setColor(sf::Color::White);
        mode2.setColor(sf::Color::White);
        mode3.setColor(sf::Color::White);
        mode4.setColor(sf::Color::White);
        mode5.setColor(sf::Color::White);
#endif
        //title.setOutlineThickness(1.0f);
        //title.setPosition(300, 200);
        auto position = window.getSize();

        int height = mode1.getGlobalBounds().height;

        title.setPosition(sf::Vector2f(position.x/2 - title.getGlobalBounds().width / 2, 50));
        mode1.setPosition(sf::Vector2f(350, 200 + 1*(height + 30)));
        mode2.setPosition(sf::Vector2f(350, 200 + 2*(height + 30)));
        mode3.setPosition(sf::Vector2f(350, 200 + 3*(height + 30)));
        mode4.setPosition(sf::Vector2f(350, 200 + 4*(height + 30)));
        mode5.setPosition(sf::Vector2f(350, 200 + 5*(height + 30)));
        

        window.clear(sf::Color::Black);

        window.draw(title);
        window.draw(mode1);
        window.draw(mode2);
        window.draw(mode3);
        window.draw(mode4);
        window.draw(mode5);
    }

    Transition onProcessInput(Input& input) override;
};

#endif /* ENDSTATE_HPP */
