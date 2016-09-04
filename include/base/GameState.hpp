/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

namespace base {
    template <typename I, typename R>
    class GameState {
     public:
        struct Transition;
        using Renderer = R;
        using Input = I;

        void periodicUpdate();
        void updateRenderer(Renderer&);
        Transition processInput(Input&);

     private:
        virtual void onPeriodicUpdate();
        virtual void onUpdateRenderer(Renderer&) = 0;
        virtual Transition onProcessInput(Input&) = 0;
    };

    template <typename I, typename R>
    struct GameState<I,R>::Transition {
        enum class Type { SELF, STORE, REPLACE, RESTORE, CLOSE };
        Type type;
        GameState<I,R>* state;
    };    
}

#include "GameState.ipp"

#endif /* GAME_STATE_HPP */