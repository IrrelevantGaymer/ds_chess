#include "board.h"

bool Board::Board::is_piece_at_index(Move::Index index) const {
    return this->board[index].has_value();
}

bool Board::Board::is_piece_capturable(Move::Index index, Move::Color capturing_color) const {
    if (this->is_piece_at_index(index)) {
        return this->board[index].value().color != capturing_color;
    }
    return false;
}

bool Board::Board::can_piece_move_to_square(Move::Index index, Move::Color capturing_color) const {
    if (this->is_piece_at_index(index)) {
        return this->board[index].value().color != capturing_color;
    }
    return true;
}