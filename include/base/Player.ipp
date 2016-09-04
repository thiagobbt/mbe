/* Copyright 2016 Marleson Graf <aszdrick@gmail.com>
   Ghabriel Nunes <ghabriel.nunes@gmail.com> */

template<typename G, typename I, typename C>
void base::Player<G,I,C>::periodicUpdate() {
    onPeriodicUpdate();
}

template<typename G, typename I, typename C>
C base::Player<G,I,C>::processInput(G& game, I& input) {
    return onProcessInput(game, input);
}

template<typename G, typename I, typename C>
void base::Player<G,I,C>::onPeriodicUpdate() { }