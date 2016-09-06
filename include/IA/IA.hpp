#ifndef IA_HPP
#define IA_HPP

#include "Utilities.hpp"
#include "multimedia/Gomoku.hpp"

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

#endif // IA_HPP
