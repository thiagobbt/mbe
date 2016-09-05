#include <iostream>
#include "mbe/Engine.hpp"
#include "multimedia/Gomoku.hpp"

enum class Casa {
    VAZIA,
    BRANCA,
    PRETA
};

enum class Direction {
    NW, N, NE,
    W,     E,
    SW, S, SE
};

using Board = Casa[15][15];

// Forward declaration necessária
class Demo;
class EndState;

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
        // Percorre o tabuleiro em busca de uma casa vazia, assim que a encontra
        // joga.
        int x = 0;
        for (auto &row : board) {
            int y = 0;
            //std::cout << type_name<decltype(row)>() << std::endl;
            for (auto place : row) {
                if (place == Casa::VAZIA) {
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


class EndState : public mbe::Game::State {
 public:
    EndState(Casa cor = Casa::PRETA) : corGanhador(cor){}

 private:
    Casa corGanhador;

    void onPeriodicUpdate() override {
    }

    void onUpdateRenderer(Renderer& window) override {
        std::string texto;

        if (corGanhador == Casa::BRANCA) {
            texto = "Branco ganhou!";
        } else if (corGanhador == Casa::PRETA) {
            texto = "Preto ganhou!";
        } else {
            texto = "Empate!";
        }

        // Declare and load a font
        sf::Font font;
        font.loadFromFile("arial.ttf");
        // Create a text
        sf::Text text(texto, font);
        text.setCharacterSize(30);
        text.setStyle(sf::Text::Bold);
        //text.setOutlineColor(sf::Color::White);
        text.setFillColor(sf::Color::White);
        //text.setOutlineThickness(1.0f);
        //text.setPosition(300, 200);
        auto position = window.getSize();
        text.setPosition(sf::Vector2f(position.x/2 - text.getGlobalBounds().width / 2, position.y/2));
        window.clear(sf::Color::Black);

        // Desenha
        window.draw(text);
    }

    Transition onProcessInput(Input& input) override;
};

class Demo : public mbe::Game::State {
 public:
    Demo(DemoPlayer&& p1 = DemoPlayer(),
         DemoPlayer&& p2 = DemoPlayer()) 
    : players{std::move(p1), std::move(p2)} {
        for (auto& a : board) {
            for (auto& b : a) {
                b = Casa::VAZIA;
            }
        }
    }

    const Board& getBoard() const { return board; }

 private:
    DemoGraphics graphics;
    std::array<DemoPlayer, 2> players;
    short currentPlayer = 0;
    Board board;
    int jogadas = 0;

    int* getPos(int row, int col, Direction d) {
        switch (d) {
            case Direction::NW:
                return new int[2]{row-1, col-1};
            case Direction::N:
                return new int[2]{row-1, col};
            case Direction::NE:
                return new int[2]{row-1, col+1};
            case Direction::W:
                return new int[2]{row, col-1};
            case Direction::E:
                return new int[2]{row, col+1};
            case Direction::SW:
                return new int[2]{row+1, col-1};
            case Direction::S:
                return new int[2]{row+1, col};
            case Direction::SE:
                return new int[2]{row+1, col+1};
        }
    }

    bool detectaFimDeJogo(int row, int col, Casa cor) {
        int tamSeq = detectarSequencias(row, col, cor, 0);

        if (detectarSequencias(row, col, cor, 1) == 4) {
            std::cout << "DETECTEI 4" << std::endl;
        }
        
        if (tamSeq == 5) return true;
        return false;
        
    }

    /*int detectarSequencias2(int row, int col, Casa cor, int tolerancia, Direction d, int count, int pulados) {
        int[] nextPos = getPos(row, col, d);
        if (board[nextPos[0]][nextPos[1]] == cor) {
            count++;
        }
    }*/

    int detectarSequencias(int row, int col, Casa cor, int tolerancia) {
        int count_linha = 1;
        int count_coluna = 1;
        int count_diag1 = 1;
        int count_diag2 = 1;
        int pulados = 0;

        for (int curr_col = col-1; curr_col >= std::max(0, curr_col-4-tolerancia); curr_col--) {
            if (board[row][curr_col] == cor) {
                count_linha++;
            } else if (board[row][curr_col] == Casa::VAZIA) {
                pulados++;
                if (pulados > tolerancia) {
                    break;
                }
            } else {
                break;
            }
        }

        for (int curr_col = col+1; curr_col <= std::min(14, curr_col+4+tolerancia); curr_col++) {
            if (board[row][curr_col] == cor) {
                count_linha++;
            } else if (board[row][curr_col] == Casa::VAZIA) {
                pulados++;
                if (pulados > tolerancia) {
                    break;
                }
            } else {
                break;
            }
        }

        pulados = 0;

        for (int curr_row = row-1; curr_row >= std::max(0, curr_row-4-tolerancia); curr_row--) {
            if (board[curr_row][col] == cor) {
                count_coluna++;
            } else if (board[curr_row][col] == Casa::VAZIA) {
                pulados++;
                if (pulados > tolerancia) {
                    break;
                }
            } else {
                break;
            }
        }

        for (int curr_row = row+1; curr_row <= std::min(14, curr_row+4+tolerancia); curr_row++) {
            if (board[curr_row][col] == cor) {
                count_coluna++;
            } else if (board[curr_row][col] == Casa::VAZIA) {
                pulados++;
                if (pulados > tolerancia) {
                    break;
                }
            } else {
                break;
            }
        }

        pulados = 0;

        for (int curr_row = row-1, curr_col = col-1; 
              (curr_row >= std::max(0, curr_row-4-tolerancia)) && 
              (curr_col >= std::max(0, curr_col-4-tolerancia)); curr_row--, curr_col--) {
            
            if (board[curr_row][curr_col] == cor) {
                count_diag1++;
            } else if (board[curr_row][curr_col] == Casa::VAZIA) {
                pulados++;
                if (pulados > tolerancia) {
                    break;
                }
            } else {
                break;
            }
        }

        for (int curr_row = row+1, curr_col = col+1; 
              (curr_row <= std::min(14, curr_row+4+tolerancia)) && 
              (curr_col <= std::min(14, curr_col+4+tolerancia)); curr_row++, curr_col++) {
            
            if (board[curr_row][curr_col] == cor) {
                count_diag1++;
            } else if (board[curr_row][curr_col] == Casa::VAZIA) {
                pulados++;
                if (pulados > tolerancia) {
                    break;
                }
            } else {
                break;
            }
        }

        pulados = 0;

        for (int curr_row = row-1, curr_col = col+1; 
              (curr_row >= std::max(0, curr_row-4-tolerancia)) && 
              (curr_col <= std::min(14, curr_col+4+tolerancia)); curr_row--, curr_col++) {
            
            if (board[curr_row][curr_col] == cor) {
                count_diag2++;
            } else if (board[curr_row][curr_col] == Casa::VAZIA) {
                pulados++;
                if (pulados > tolerancia) {
                    break;
                }
            } else {
                break;
            }
        }

        for (int curr_row = row+1, curr_col = col-1; 
              (curr_row <= std::min(14, curr_row+4+tolerancia)) && 
              (curr_col >= std::max(0, curr_col-4-tolerancia)); curr_row++, curr_col--) {
            
            if (board[curr_row][curr_col] == cor) {
                count_diag2++;
            } else if (board[curr_row][curr_col] == Casa::VAZIA) {
                pulados++;
                if (pulados > tolerancia) {
                    break;
                }
            } else {
                break;
            }
        }

        return std::max(std::max(count_coluna, count_linha), std::max(count_diag1, count_diag2));
    }

    // Agora nosso onUpdateRenderer chama o update de graphics
    void onUpdateRenderer(Renderer& window) override {
        graphics.update(*this, window);
    }

    // Aqui, mandamos o input para player e recebemos uma gm::Position de
    // resposta. Ela representa a jogada do player.
    Transition onProcessInput(Input& input) override {
        auto move = players[currentPlayer].processInput(board, input);
        // Jogadas fora do tabuleiro são respondidas com -1, -1
        if (move != gm::Position{-1, -1}) {
            // Verifica se a posição está desocupada
            if (board[move.row][move.column] == Casa::VAZIA) {
                jogadas++;
                // Marca a posição como jogada do currentPlayer
                board[move.row][move.column] = static_cast<Casa>(currentPlayer+1);

                bool jogadorGanhou = detectaFimDeJogo(move.row, move.column, static_cast<Casa>(currentPlayer+1));

                if (jogadorGanhou) {   
                    std::cout << "Fim de Jogo. Jogador " << currentPlayer << " venceu!" << std::endl;
                    return {Transition::Type::STORE, new EndState(static_cast<Casa>(currentPlayer+1))};
                } else if (jogadas >= 225) {
                    std::cout << "Fim de Jogo. Empate!" << std::endl;
                    return {Transition::Type::STORE, new EndState(Casa::VAZIA)};
                }

                // Troca o current player
                currentPlayer = 1 - currentPlayer;
            }
        }
        return {Transition::Type::SELF, this};
    }
};

mbe::Game::State::Transition EndState::onProcessInput(Input& input) {
    for (auto event : input) {
        if (event.type == sf::Event::MouseButtonPressed) {
            return {Transition::Type::REPLACE, new Demo(DemoPlayer(), DemoPlayer(new Skynet()))};
        }
    }
    return {Transition::Type::SELF, this};
}

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
    // Desenhamos o tabuleiro
    drawBoard(renderer);
    // Pegamos o tabuleiro
    auto &stones = demo.getBoard();
    // Loop grotesco para descobrir onde estão as pedras e desenhá-las
    int x = 0;
    for (auto &rowOfStones : stones) {
        int y = 0;
        for(auto &stone : rowOfStones) {
            if (stone != Casa::VAZIA) {
                sf::Color color = stone == Casa::PRETA ? GMTraits::BLACK_COLOR : GMTraits::WHITE_COLOR;
                // gm::Stone é uma struct com uma gm::Position ({x, y}) e uma sf::Color
                drawStone({{x, y}, color}, renderer);
            }
            y++;
        }
        x++;
    }
}

int main(int argc, char** argv) {
    auto *ia = new Skynet();
    auto *gameState = new Demo(DemoPlayer(), DemoPlayer(ia));
    auto *game = new mbe::Game(gameState);

    mbe::Engine e(game, 1024, 768, "Demo");
    e.run();

    return 0;
}