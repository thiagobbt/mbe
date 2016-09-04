/* Copyright 2016 Marleson Graf <aszdrick@gmail.com> */

#include <iostream>
#include "mbe/Engine.hpp"

/** Nessa segunda parte, é mostrado como criar uma classe Game,
 * estrutura que contém e gerencia os estados.
 * 
 * A class MyGame deriva a mbe::Game. mbe::Game e todas as classes
 * associadas nada mais são que especificações de templates para instanciar
 * as estruturas genéricas em base. Fazer herança de mbe garante
 * compatibilidade plena com o que a Engine espera receber.
 *
 * A superclasse base::Game faz todo o gerenciamento de troca de estados,
 * mas, se quiser, é possível acessar métodos protected para alterar a fila
 * de estados diretamente (popState(State* const), pushState(State* const)
 * e replaceState(State* const)).
 *
 * Em um jogo simples, a maior parte das responsabilidades de Game está
 * gerenciada na superclasse. A maior vantagem de implementar uma classe
 * derivada de game, é poder processar entradas, gráficos e lógica num
 * contexto global (independente de estados).
**/
class MyGame : public mbe::Game {
 public:
    // Um construtor padrão para simplificar a chamada no main
    MyGame();
 private:
    // Aqui, o padrão se repete, os mesmos três métodos de State.
    // Note que para Game, os três são opcionais. Por isso é possível
    // utilizar a superclasse diretamente, como no exemplo anterior,
    // já que não há qualquer método puramente virtual.

    void onPeriodicUpdate() override {
        std::cout << "[CALL] MyGame::onPeriodicUpdate" << std::endl;
    }

    // onUpdateRenderer é chamado sempre APÓS a chamada do mesmo em
    // State. Isso garante que qualquer mudança gráfica feita por esta clase
    // seja respeitada, i.e., nenhum State do jogo pode alterar.
    void onUpdateRenderer(Renderer& window) override {
        std::cout << "[CALL] MyGame::onUpdateRenderer" << std::endl;
        // Essa chamada apagará o círculo desenhado por Demo
        window.clear(sf::Color::Black);
    }

    // onProcessInput é chamado sempre ANTES da chamada do mesmo em
    // State. Isso garante que qualquer evento de alta importância
    // seja tratado por Game antes de propagar o resto dos eventos.
    void onProcessInput(Input& input) override {
        std::cout << "[CALL] MyGame::onProcessInput" << std::endl;
    }
};

// Note a mudança da herança de mbe::Game::State para MyGame::State
// Na prática, os dois são a mesma coisa, mas utilizar essa sintaxe
// garante coerência entre Demo e MyGame, mesmo que os templates de
// MyGame mudem.
class Demo : public MyGame::State {
 private:
    void onPeriodicUpdate() override {
        std::cout << "[CALL] Demo::onPeriodicUpdate" << std::endl;
    }

    void onUpdateRenderer(Renderer& window) override {
        std::cout << "[CALL] Demo::onUpdateRenderer" << std::endl;
        auto shape = sf::CircleShape(100, 50);
        shape.setPosition(300, 200);
        shape.setOutlineThickness(10);
        shape.setOutlineColor(sf::Color::White);
        shape.setFillColor(sf::Color::Black);
        window.clear(sf::Color::Black);
        window.draw(shape);
    }

    Transition onProcessInput(Input& input) override {
        std::cout << "[CALL] Demo::onProcessInput" << std::endl;
        return {Transition::Type::SELF, this};
    }
};

// Implementação do construtor padrão de MyGame
// Inicializa a fila de estados com um novo Demo
MyGame::MyGame() : mbe::Game(new Demo()) { }

int main(int argc, char** argv) {
    // Agora só precisamos criar um novo MyGame...
    mbe::Engine e(new MyGame(), 800, 600, "Demo");
    // ...e rodar a engine
    e.run();

    return 0;
}