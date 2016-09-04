/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

#ifndef PLAYER_HPP
#define PLAYER_HPP

namespace base {
    template<typename Game, typename InputProvider, typename Command = void>
    class Player {
     public:
        using Board = Game;
        using Move = Command;
        using Input = InputProvider;

        void periodicUpdate();
        Move processInput(Game&, InputProvider&);
     private:
        virtual void onPeriodicUpdate();
        virtual Move onProcessInput(Board&, Input&) = 0;
    };
}

#include "Player.ipp"

#endif /* PLAYER_HPP */