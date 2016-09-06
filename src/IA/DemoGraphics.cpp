#include "IA/DemoGraphics.hpp"
#include "IA/Demo.hpp"

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