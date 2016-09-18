/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

#ifndef GM_GOMOKU_BOARD_HPP
#define GM_GOMOKU_BOARD_HPP

#include "base/Component.hpp"
#include "base/Player.hpp"
#include <SFML/Graphics.hpp>
#include <list>

// Gomoku Multimedia
namespace gm {
    struct Position {
        int row;
        int column;
    };

    struct Stone {
        Position position;
        sf::Color color;
    };

    using Renderer = sf::RenderWindow;
    using GameInput = std::list<sf::Event>;
    using PlayerInput = Position;

    template<typename Board, typename Move>
    using Player = base::Player<Board, GameInput, Move>;

    template<typename GameState>
    class Graphics : public base::Component<GameState, Renderer> {
     protected:
        void drawStone(const Stone&, Renderer&) const;
        void drawBoard(Renderer&) const;
    };

    template<typename GameState>
    class PlayerHandler : public base::Component<GameState, GameInput, PlayerInput> {
     protected:
        Position pixelToPosition(float, float);
        Position pixelToBoardPosition(float, float);
     private:
        virtual PlayerInput doUpdate(GameState&, gm::GameInput&);
    };

    inline bool operator==(const Position& lhs, const Position& rhs) {
        return lhs.row == rhs.row && lhs.column == rhs.column;
    }

    inline bool operator!=(const Position lhs, const Position& rhs) {
        return !(lhs == rhs);
    }

    inline std::ostream& operator<<(std::ostream& stream, const Position& p) {
        stream << "{" << p.row << ", " << p.column << "}";
        return stream;
    }
}

#include "Gomoku.ipp"

#endif /* MTM_GOMOKU_BOARD_HPP */
