#ifndef DEMOGRAPHICS_HPP
#define DEMOGRAPHICS_HPP

class Demo;

// Para uma partida de Gomoku, multimedia/Gomoku.hpp define um Component chamado
// Graphics, que recebe um template para definir o tipo do objeto para o qual 
// ele provê os gráficos. Nós utilizaremos um Graphics<Demo>. Note que é necessário
// herdar esse componente e definir um método chamado onUpdate, o qual sempre é chamado
// quando executado graphics.update(...).
class DemoGraphics : public gm::Graphics<Demo> {
 private:
    // Método que realiza o update propriamente dito
    void doUpdate(Demo&, gm::Renderer&) override;
};

#endif /* DEMOGRAPHICS_HPP */

