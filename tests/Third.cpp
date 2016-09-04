/* Copyright 2016 Marleson Graf <aszdrick@gmail.com> */

#include <iostream>
#include "mbe/Engine.hpp"
#include "multimedia/Gomoku.hpp"

/** Aqui é mostrado como utilizar o tipo Component, mais especificamente,
 * aqueles definidos em multimedia/Gomoku.hpp. Com esses recursos, é possível
 * criar um jogo de Gomoku com suporte a IA.
 *
 * Um Component é uma abstração sobre as funcionalidades de uma dada classe.
 * Seu uso tem como objetivo desacoplar essas funcionalidades da classe principal.
**/

// Board é uma representação simples de um tabuleiro de Gomoku.
// Um vetor de vetores de unsigned int, onde 0 = player preto, 1 = player branco,
// e 2 = desocupado.
using Board = std::vector<std::vector<unsigned>>;

// Forward declaration necessária
class Demo;

// Skynet é nossa IA, ela herda de um componente definido em multimedia/Gomoku.hpp,
// o PlayerHandler. O parâmetro de template dessa classe define o tipo de objeto que
// recebe além do gm::GameInput. Uma observação importante é que PlayerHandler não
// é abstrato, você pode fazer um 'new PlayerHandler<Board>' e isto criará um handler
// para jogador humano.
//                                      ISTO
//                                        v
class Skynet : public gm::PlayerHandler<Board> {
 private:
// O método doUpdate é chamado sempre que chamado handler.update()
//                     DEFINE ISTO
//                          v
    gm::Position doUpdate(Board& board, gm::GameInput& input) override {
        std::cout << "[CALL] Skynet::doUpdate" << std::endl;
        // Percorre o tabuleiro em busca de uma casa vazia, assim que a encontra
        // joga.
        int x = 0;
        for (auto row : board) {
            int y = 0;
            for (auto place : row) {
                if (place == 2) {
                    return {x, y};
                }
                y++;
            }
            x++;
        }
        return {-1, -1};
    }
};

// DemoPlayer é nosso Player, herdando de gm::Player, o qual requer dois templates:
// um objeto sobre o qual pode analisar sua jogada (Board) e a saída do processamento
// das entradas do jogo (gm::Position). Um jogador humano nunca precisa analisar o 
// objeto Board, enquanto um jogador artificial nunca precisa analisar as entradas. 
// Como não se sabe o que o handler é (IA ou humano), sempre são passados 
// os dois argumentos.
// 
//                                  ISTO(1)    ISTO(2) 
//                                     v         v
class DemoPlayer : public gm::Player<Board, gm::Position> {
 public:
    // Esse using tem apenas a utilidade de diminuir a verbosidade das próximas
    // referências à gm::PlayerHandler<Board>
    using Handler = gm::PlayerHandler<Board>;

    // Construtor de DemoPlayer recebe o handler, cujo default é ser humano
    DemoPlayer(Handler * const handler = new Handler())
     : handler(handler) { }

 private:
    // É essencial que o PlayerHandler seja guardado como PONTEIRO, caso contrário,
    // sempre se comportará como humano, mesmo que seja inicializado com o Skynet.
    std::unique_ptr<Handler> handler;

// onProcessInput é chamado através de player.processInput(...) e é nele que é realizado
// o processamento das entradas do jogo. Nesse caso, como usamos um componente para fazer
// o serviço, basta chamar o update do mesmo.
// 
//  DEFINE ISTO(2)          DEFINE ISTO(1)
//        v                       v
    gm::Position onProcessInput(Board& board, gm::GameInput& input) {
        std::cout << "[CALL] DemoPlayer::onProcessInput" << std::endl;
        gm::Position position = handler->update(board, input);
        return position;
    }
};


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

// Nossa classe Demo, com pequenas mudanças. MyGame não era necessário, portanto foi
// removido, e agora a herança de Demo é direta de mbe::Game::State
class Demo : public mbe::Game::State {
 public:
    // O construtor recebe um array de 2 players, com default definido como
    // dois DemoPlayers defaults (i.e., players humanos)
    Demo(const std::array<DemoPlayer, 2>& players = {DemoPlayer(), DemoPlayer()}) 
    : players(players) {
        for (int i = 0; i < 15; i++) {
            board.push_back(std::vector<unsigned>(15, 2));
        }
    }

    // Um getter para simplificar as coisas, mas que poderia ser EVITADO
    const Board& getBoard() const { return board; }

 private:
    // O componente gráfico
    DemoGraphics graphics;
    // O array de jogadores
    std::array<DemoPlayer, 2> players;
    // Variável para guardar o jogador atual
    short currentPlayer = 0;
    // O tabuleiro
    Board board;

    // Agora nosso onUpdateRenderer chama o update de graphics
    void onUpdateRenderer(Renderer& window) override {
        std::cout << "[CALL] Demo::onProcessUpdateRenderer" << std::endl;
        graphics.update(*this, window);
    }

    // Aqui, mandamos o input para player e recebemos uma gm::Position de
    // resposta. Ela representa a jogada do player.
    Transition onProcessInput(Input& input) override {
        std::cout << "[CALL] Demo::onProcessInput" << std::endl;
        auto move = players[currentPlayer].processInput(board, input);
        // Jogadas fora do tabuleiro são respondidas com -1, -1
        if (move != gm::Position{-1, -1}) {
            // Verifica se a posição está desocupada
            if (board[move.row][move.column] == 2) {
                // Marca a posição como jogada do currentPlayer
                board[move.row][move.column] = currentPlayer;
            }
            // Troca o current player
            currentPlayer = 1 - currentPlayer;
        }
        return {Transition::Type::SELF, this};
    }
};

// Implementação do método doUpdate de DemoGraphics (feito aqui em baixo porque
// necessitava do tipo Demo completamente definido, lembra da forward declaration?)
// 
// Aqui é necessário tratar a informação das pedras que serão desenhadas.
// multimedia/Gomoku.hpp apenas provê duas funções, drawBoard(gm::Renderer&) e 
// drawStone(const gm::Stone&, gm::Renderer&). Como a forma que a informação das pedras
// é guardada depende da implementação do jogo, esse método precisa ser definido pelo
// usuário. No nosso exemplo, aqui convertemos as informações da board para gm::Stone,
// passando cada uma delas para o drawStone...
void DemoGraphics::doUpdate(Demo& demo, gm::Renderer& renderer) {
    std::cout << "[CALL] DemoGraphics::onUpdateRenderer" << std::endl;
    // Desenhamos o tabuleiro
    drawBoard(renderer);
    // Pegamos o tabuleiro
    auto stones = demo.getBoard();
    // Loop grotesco para descobrir onde estão as pedras e desenhá-las
    int x = 0;
    for (auto rowOfStones : stones) {
        int y = 0;
        for(auto stone : rowOfStones) {
            if (stone < 2) {
                sf::Color color = stone == 0 ? GMTraits::BLACK_COLOR : GMTraits::WHITE_COLOR;
                // gm::Stone é uma struct com uma gm::Position ({x, y}) e uma sf::Color
                drawStone({{x, y}, color}, renderer);
            }
            y++;
        }
        x++;
    }
}

int main(int argc, char** argv) {
    // Agora só precisamos criar um novo mbe::Game...
    mbe::Engine e(new mbe::Game(new Demo()), 1024, 768, "Demo");
    // ...e rodar a engine
    e.run();

    return 0;
}