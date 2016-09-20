#include "IA/Board.hpp"

PlayerType Board::getPosition(gm::Position pos) {
    return board[pos.row][pos.column];
}

PlayerType Board::getOpponent() {
    return currentPlayer == PlayerType::WHITE? PlayerType::BLACK : PlayerType::WHITE;
}

PlayerType Board::getCurrentPlayer() {
    return currentPlayer;
}

gm::Position Board::getLastPlay() {
    return lastPlay;
}

uint Board::getNumPlays() {
    return numPlays;
}

std::unordered_set<gm::Position> getAdjacent(gm::Position p) {
    std::unordered_set<gm::Position> adjacent;
    if (p.row > 0 ) {
        if (p.column > 0) adjacent.insert({p.row-1, p.column-1});
        adjacent.insert({p.row-1, p.column});
        if (p.column < 14) adjacent.insert({p.row-1, p.column+1});
    }

    if (p.column > 0) adjacent.insert({p.row, p.column-1});
    if (p.column < 14) adjacent.insert({p.row, p.column+1});

    if (p.row < 14) {
        if (p.column > 0) adjacent.insert({p.row+1, p.column-1});
        adjacent.insert({p.row+1, p.column});
        if (p.column < 14) adjacent.insert({p.row+1, p.column+1});
    }

    return adjacent;
}

std::unordered_set<gm::Position> Board::getChildren() {
    /*
    std::list<gm::Position> children;
    for (int i = 0; i < 225; i++) {
        int row = i / 15;
        int col = i % 15;
        if (getPosition({row, col}) == PlayerType::EMPTY) {
            children.push_back({row, col});
        }
    }*/

    std::unordered_set<gm::Position> children;

    if (plays.size() == 0) {
        children.insert({7, 7});
        return children;
    }

    for (auto play : plays) {
        for (auto p : getAdjacent(play)) {
            if (getPosition(p) == PlayerType::EMPTY)
                children.insert(p);
        }
    }
    return children;
}

bool Board::isGameEnded() {
    return gameEnded;
}

Board::Sequencias Board::detectarSequencias(gm::Position pos, int tolerancia) {
    PlayerType cor = getPosition(pos);

    int count_linha = 1;
    int count_coluna = 1;
    int count_diag1 = 1;
    int count_diag2 = 1;
    int pulados = 0;

    int row = pos.row;
    int col = pos.column;

    for (int curr_col = col-1; curr_col >= std::max(0, curr_col-4-tolerancia); curr_col--) {
        if (board[row][curr_col] == cor) {
            count_linha++;
        } else if (board[row][curr_col] == PlayerType::EMPTY) {
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
        } else if (board[row][curr_col] == PlayerType::EMPTY) {
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
        } else if (board[curr_row][col] == PlayerType::EMPTY) {
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
        } else if (board[curr_row][col] == PlayerType::EMPTY) {
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
        } else if (board[curr_row][curr_col] == PlayerType::EMPTY) {
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
        } else if (board[curr_row][curr_col] == PlayerType::EMPTY) {
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
        } else if (board[curr_row][curr_col] == PlayerType::EMPTY) {
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
        } else if (board[curr_row][curr_col] == PlayerType::EMPTY) {
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

bool Board::checkGameEnded(gm::Position pos) {
    if (numPlays == 225) return true;

    Sequencias seq = detectarSequencias(pos, 0);
    int maxTamSeq = std::max(std::max(seq.coluna, seq.linha), std::max(seq.diag_primaria, seq.diag_secundaria));

    if (maxTamSeq >= 5) {
        return true;
    }

    return false;
}

bool Board::play(gm::Position pos, bool debug) {
    if (pos.row < 0 || pos.row > 14 || pos.column < 0 || pos.column > 14) {
        std::cout<<"Board::play " << currentPlayer << " " << pos << " error is not in bounds"<<std::endl;
        return false;
    }

    if (getPosition(pos) != PlayerType::EMPTY) {
        std::cout<<"Board::play " << currentPlayer << " " << pos << " error is not VAZIA"<<std::endl;
        return false;
    }

    if (gameEnded) {
        std::cout<<"Board::play " << currentPlayer << " " << pos << " error game is ended"<<std::endl;
        return false;
    }

    numPlays++;
    lastPlay = pos;
    plays.push_back(pos);
    board[pos.row][pos.column] = currentPlayer;
    gameEnded = checkGameEnded(lastPlay);
    currentPlayer = getOpponent();

    return true;
}
