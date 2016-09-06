#ifndef DEMO_HPP
#define DEMO_HPP

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

#endif /* DEMO_HPP */

