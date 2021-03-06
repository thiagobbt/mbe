/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

#include <cmath>
#include "Traits.hpp"

template<typename G>
void gm::Graphics<G>::drawStone(const Stone& stone, Renderer& window) const {
    auto shape = sf::CircleShape(GMTraits::STONE_RADIUS, 50);
    auto squareSize = GMTraits::SQUARE_SIZE;
    shape.setPosition(sf::Vector2f(
        squareSize + stone.position.column * squareSize 
        - GMTraits::STONE_RADIUS,
        squareSize + stone.position.row * squareSize 
        - GMTraits::STONE_RADIUS));

    auto w_border = GMTraits::WHITE_OUTLINE_COLOR;
    auto b_border = GMTraits::BLACK_OUTLINE_COLOR;

    shape.setOutlineThickness(GMTraits::STONE_BORDER_WIDTH);
    
    shape.setOutlineColor(stone.color == GMTraits::WHITE_COLOR ? w_border : b_border);
    
    shape.setFillColor(stone.color);

    window.draw(shape);
}

template<typename G>
void gm::Graphics<G>::drawBoard(Renderer& window) const {
    static std::vector<sf::Vertex> lines;
    auto boardDimension = GomokuTraits::BOARD_DIMENSION;
    if (lines.empty()) {
        auto squareSize = GMTraits::SQUARE_SIZE;
        auto boardStart = GMTraits::BORDER_WIDTH;
        auto boardEnd = boardStart + (boardDimension - 1) * squareSize;
        auto offset = [=](unsigned i) {
            return boardStart + i * squareSize;
        };
        
        for (unsigned i = 0; i < boardDimension; i++) {
            lines.emplace_back(sf::Vector2f(boardStart, offset(i)));
            lines.emplace_back(sf::Vector2f(boardEnd, offset(i)));
        }

        for (unsigned i = 0; i < boardDimension; i++) {
            lines.emplace_back(sf::Vector2f(offset(i), boardStart));
            lines.emplace_back(sf::Vector2f(offset(i), boardEnd));
        }
    }

    window.clear(sf::Color::Black);
    window.draw(&lines.front(), 4 * boardDimension, sf::Lines);
}

template<typename G>
gm::Position gm::PlayerHandler<G>::pixelToPosition(float x, float y) {
    auto boardStart = GMTraits::BORDER_WIDTH;
    auto squareSize = GMTraits::SQUARE_SIZE;
    int column = round((x - boardStart) / squareSize);
    int row = round((y - boardStart) / squareSize);
    return {row, column};
}

template<typename G>
gm::Position gm::PlayerHandler<G>::pixelToBoardPosition(float x, float y) {
    auto position = pixelToPosition(x, y);
    int boardDimension = GomokuTraits::BOARD_DIMENSION;
     if (position.row >= 0 && position.column >= 0 
            && position.row < boardDimension 
            && position.column < boardDimension) {
        return position;
     }
     return {-1, -1};
}

template<typename G>
gm::Position gm::PlayerHandler<G>::doUpdate(G&, gm::GameInput& events) {
    gm::Position position = {-1, -1};
    for (auto event : events) {
        if (event.type == sf::Event::MouseButtonPressed) {
            position = pixelToBoardPosition(event.mouseButton.x, event.mouseButton.y);
        }
    }
    return position;
}