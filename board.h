#ifndef BOARD_H
#define BOARD_H

#include <variant>

#include "move.h"

namespace Board {
    class Board {
    public:
        std::optional<Move::Piece> board[64];
        std::optional<size_t> en_passant;
        Move::Color current_player;

        //returns true if a piece exists at the index
        bool is_piece_at_index(Move::Index index) const;
        //returns true if a piece exists at the index and if the piece is the opposite color to capturing_color
        bool is_piece_capturable(Move::Index index, Move::Color capturing_color) const;
        //returns true if a piece doesn't exist at the index OR if one does, that its color is opposite to capturing_color
        bool can_piece_move_to_square(Move::Index index, Move::Color capturing_color) const;
        
        bool is_valid_move(Move::Move move);
        MoveResult make_move(Move::Move move);
        MoveResult unmake_move(Move::Move move);
        std::tuple<size_t, size_t> generate_attacked_spaces();
    };

    typedef std::variant<SuccessfulOperation, MoveError> MoveResult;

    struct SuccessfulOperation;

    enum MoveError {
        InvalidMove,
        IndexOutOfBounds,
        KingLeftInCheck
    };
}

#endif