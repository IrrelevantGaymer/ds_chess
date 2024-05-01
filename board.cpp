#include <optional>

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

Board::MoveResult Board::Board::make_move(Move::Move move) {
    if (!this->is_valid_move(move)) {
        return MoveResult(MoveError::InvalidMove);
    }
    if (move.from >= 64 || move.to >= 64) {
        return MoveResult(MoveError::IndexOutOfBounds);
    }

    auto moved_two_squares_forward = (move.from < move.to ? move.to - move.from : move.from - move.to) == 16;
    auto was_piece_pawn = this->board[move.from].has_value() 
        && this->board[move.from].value().piece_type == Move::PieceType::Pawn;
    auto piece_was_on_correct_rank = this->current_player == Move::Color::White ?
       (move.from >= 8 && move.from < 16) 
       :(move.from >= 48 && move.from < 56);
    if (moved_two_squares_forward && was_piece_pawn && piece_was_on_correct_rank) {
        this->en_passant = std::optional(move.to);
    } else {
        this->en_passant = std::nullopt;
    }

    //TODO Add King in Check checks to see if move is legal
    //TODO Maybe have check in is_legal_move

    move.capture = this->board[move.to];
    this->board[move.to] = this->board[move.from];
    this->board[move.from] = std::nullopt;

    this->current_player = Move::swap(this->current_player);

    return MoveResult(SuccessfulOperation());
}

Board::MoveResult Board::Board::unmake_move(Move::Move move) {
    this->board[move.to] = move.capture;
    this->board[move.from] = this->board[move.to];
    this->current_player = Move::swap(this->current_player);
}

std::tuple<size_t, size_t> Board::Board::generate_attacked_spaces() {
    size_t white_attack = 0;
    size_t black_attack = 0;

    for (size_t i = 0; i < std::size(this->board); i++) {
        if (this->board[i].has_value()) {
            //TODO
        }
    }
}