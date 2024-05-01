#include <optional>

#include "board.h"
#include "move.h"

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

std::optional<Move::Index> Board::Board::get_king_index(Move::Color color) const {
    auto it = std::find(
        this->board.begin(), 
        this->board.end(), 
        Move::Piece(color, Move::PieceType::King)
    );
    if (it != this->board.end()) {
        return std::optional(std::distance(this->board.begin(), it));
    }
}

Board::MoveResult Board::Board::is_valid_move(Move::Move *move) {
    if (!this->board[move->from].has_value()) {
        return MoveResult(MoveError::NoPieceToMove);
    }

    if (move->from >= 64 || move->to >= 64) {
        return MoveResult(MoveError::IndexOutOfBounds);
    }

    Move::Piece piece = this->board[move->from].value();
    auto possible_moves = Move::Piece::generate_legal_moves(this, piece.color);

    if (std::find(possible_moves.begin(), possible_moves.end(), move->to) == possible_moves.end()) {
        return MoveResult(MoveError::InvalidMove);
    } else if (piece.color != this->current_player) {
        return MoveResult(MoveError::InvalidMove);
    }
    
    move->capture = this->board[move->to];
    this->board[move->to] = this->board[move->from];
    this->board[move->from] = std::nullopt;

    Move::swap_ptr(&this->current_player);

    auto king_index = this->get_king_index(piece.color);
    if (!king_index.has_value()) {
        return MoveResult(MoveError::NoKing);
    }
    auto potential_captures = Move::Piece::generate_capture_moves(this, piece.color);
    if (std::find(potential_captures.begin(), potential_captures.end(), king_index.value()) != potential_captures.end()) {
        return MoveResult(MoveError::KingLeftInCheck);
    }

    this->board[move->to] = move->capture;
    this->board[move->from] = this->board[move->to];
    Move::swap_ptr(&this->current_player);

    return MoveResult(SuccessfulOperation {});
}

Board::MoveResult Board::Board::make_move(Move::Move *move) {
    auto moved_two_squares_forward = (move->from < move->to ? move->to - move->from : move->from - move->to) == 16;
    auto was_piece_pawn = this->board[move->from].has_value() 
        && this->board[move->from].value().piece_type == Move::PieceType::Pawn;
    auto piece_was_on_correct_rank = this->current_player == Move::Color::White ?
       (move->from >= 8 && move->from < 16) 
       :(move->from >= 48 && move->from < 56);
    if (moved_two_squares_forward && was_piece_pawn && piece_was_on_correct_rank) {
        this->en_passant = std::optional(move->to);
    } else {
        this->en_passant = std::nullopt;
    }

    move->capture = this->board[move->to];
    this->board[move->to] = this->board[move->from];
    this->board[move->from] = std::nullopt;

    this->current_player = Move::swap(this->current_player);

    return MoveResult(SuccessfulOperation());
}

Board::MoveResult Board::Board::unmake_move(Move::Move *move) {
    this->board[move->to] = move->capture;
    this->board[move->from] = this->board[move->to];
    this->current_player = Move::swap(this->current_player);
}

void Board::Board::print_board() const {
    for (size_t rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << " ";    
        for (size_t file = 0; file < 8; file++) {
            Move::Index index = Move::Move::coord_to_index(rank, file);
            std::optional<Move::Piece> piece = this->board[index];

            if (piece.has_value()) {
                std::cout << piece.value().to_string() << " ";
            } else {
                std::cout << "- ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "  a b c d e f g h " << std::endl;
}