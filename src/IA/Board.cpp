#include "IA/Board.hpp"
#include <algorithm>

Board::Board() {
    for (auto& a : board) {
        for (auto& b : a) {
            b = PlayerType::VAZIA;
        }
    }
}

Board::Sequencias Board::detectarSequencias(gm::Position pos, int tolerancia) {
    int count_linha = 1;
    int count_coluna = 1;
    int count_diag1 = 1;
    int count_diag2 = 1;
    int pulados = 0;

    int row = pos.row;
    int col = pos.column;
    PlayerType cor = getOpponent();

    for (int curr_col = col-1; curr_col >= std::max(0, curr_col-4-tolerancia); curr_col--) {
        if (board[row][curr_col] == cor) {
            count_linha++;
        } else if (board[row][curr_col] == PlayerType::VAZIA) {
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
        } else if (board[row][curr_col] == PlayerType::VAZIA) {
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
        } else if (board[curr_row][col] == PlayerType::VAZIA) {
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
        } else if (board[curr_row][col] == PlayerType::VAZIA) {
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
        } else if (board[curr_row][curr_col] == PlayerType::VAZIA) {
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
        } else if (board[curr_row][curr_col] == PlayerType::VAZIA) {
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
        } else if (board[curr_row][curr_col] == PlayerType::VAZIA) {
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
        } else if (board[curr_row][curr_col] == PlayerType::VAZIA) {
            pulados++;
            if (pulados > tolerancia) {
                break;
            }
        } else {
            break;
        }
    }

    return {count_coluna, count_linha, count_diag1, count_diag2};
}

bool Board::detectaFimDeJogo(gm::Position pos) {
    Sequencias seq = detectarSequencias(pos, 0);
    int maxTamSeq = std::max(std::max(seq.coluna, seq.linha), std::max(seq.diag_primaria, seq.diag_secundaria));

    if (maxTamSeq >= 5) {
        gameFinished = true;
        return true;
    }
    return false;
}
