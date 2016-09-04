/* Copyright 2016 Marleson Graf <aszdrick@gmail.com> */

#include <iostream>
#include "mbe/Engine.hpp"

/** Nessa primeira parte, é criada uma aplicação simples que desenha
 * um círculo na tela, usando o mínimo de definições necessárias.
 *
 * A classe Demo deriva um mbe::Game::State.
 * Um State corresponde a um estado do jogo, e.g.,
 * Menu, partida, pause, configurações...
 * Um jogo pode ser definido com apenas um estado inicial.
 *
 * Existem 3 métodos privados principais, dois dos quais
 * devem ser sobrescritos: onUpdateRenderer e onProcessInput.
 * O terceiro método é onPeriodicUpdate, que seria um update
 * que acontece em tempo periódico. No atual momento, esse método
 * não é periódico, i.e., é chamado quantas vezes a CPU conseguir.
**/
class Demo : public mbe::Game::State {
 public:

 private:
    // onPeriodicUpdate realiza atualizações da parte lógica do jogo,
    // teoricamente em intervalos fixos de tempo. Na prática, isso ainda
    // não foi implementado
    void onPeriodicUpdate() override {
        std::cout << "[CALL] Demo::onPeriodicUpdate" << std::endl;
    }

    // onUpdateRenderer realiza a atualização da parte gráfica do jogo.
    // Recebe como argumento a window em que é renderizado o jogo.
    // No caso de mbe::Game, a window é uma sf::RenderWindow, da
    // SFML. Renderer é definido através dos templates de base.
    void onUpdateRenderer(Renderer& window) override {
        std::cout << "[CALL] Demo::onUpdateRenderer" << std::endl;
        // Cria um novo círculo
        auto shape = sf::CircleShape(100, 50);
        // Posiciona-o onde desejar
        shape.setPosition(300, 200);
        // Define borda de 10 pixels
        shape.setOutlineThickness(10);
        // Define a cor da borda
        shape.setOutlineColor(sf::Color::White);
        // Define a cor de preenchimento
        shape.setFillColor(sf::Color::Black);
        // Limpa a window com fundo preto
        window.clear(sf::Color::Black);
        // Desenha
        window.draw(shape);
    }

    // onProcessInput realiza o processamento das entradas recebidas no 
    // intervalo entre a chamada anterior e a atual. Para o mbe::Game,
    // o Input é uma list<sf::Event>, i.e., uma lista de eventos da SFML
    Transition onProcessInput(Input& input) override {
        std::cout << "[CALL] Demo::onProcessInput" << std::endl;
        // Sempre que chamado, esse método precisa retornar uma transição.
        // Como essa demonstração tem apenas um estado, ela nunca realiza
        // transições. Isso é concretizado com o retorno de uma 
        // Transition::Type::SELF. Passar this como segundo argumento é
        // desnecessário, pode ser um nullptr.
        return {Transition::Type::SELF, this};
    }
};

int main(int argc, char** argv) {
    // Para utilizar o State definido acima, basta criar uma nova engine,
    // passando um novo mbe::Game e dando a ele o estado inicial, nesse caso
    // um new Demo(). O construtor de mbe::Game também aceita o tamanho da
    // window, largura x altura, e o nome da janela.
    mbe::Engine e(new mbe::Game(new Demo()), 800, 600, "Demo");
    // Agora é só rodar a engine
    e.run();

    return 0;
}