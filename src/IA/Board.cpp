#include "IA/Board.hpp"
#include <algorithm>

Board::Board() {
    for (auto& a : board) {
        for (auto& b : a) {
            b = Casa::VAZIA;
        }
    }
}

int* Board::detectarSequencias(int row, int col, Casa cor, int tolerancia) {
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

    return new int[4]{count_coluna, count_linha, count_diag1, count_diag2};
}

bool Board::detectaFimDeJogo(int row, int col, Casa cor) {
    int* seq = detectarSequencias(row, col, cor, 0);
    int maxTamSeq = 0;

    for (int i = 0; i < 4; i++) {
        if (seq[i] > maxTamSeq) maxTamSeq = seq[i];
    }
    
    if (maxTamSeq == 5) {
        gameFinished = true;
        return true;
    }
    return false;
}