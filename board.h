#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <iostream>
#include <optional>
#include <tuple>
#include <variant>

#include "move.h"

namespace Board {
    class Board {
    public:
        std::array<std::optional<Move::Piece>, 64> board;
        std::optional<size_t> en_passant;
        Move::Color current_player;

        //returns true if a piece exists at the index
        bool is_piece_at_index(Move::Index index) const;
        //returns true if a piece exists at the index and if the piece is the opposite color to capturing_color
        bool is_piece_capturable(Move::Index index, Move::Color capturing_color) const;
        //returns true if a piece doesn't exist at the index OR if one does, that its color is opposite to capturing_color
        bool can_piece_move_to_square(Move::Index index, Move::Color capturing_color) const;
        //gets index of the king
        std::optional<Move::Index> get_king_index(Move::Color color) const;

        MoveResult is_valid_move(Move::Move *move);
        MoveResult make_move(Move::Move *move);
        MoveResult unmake_move(Move::Move *move);
        void print_board() const;
    };

    struct SuccessfulOperation {};

    enum MoveError {
        NoPieceToMove,
        InvalidMove,
        IndexOutOfBounds,
        KingLeftInCheck,
        NoKing
    };

    typedef std::variant<SuccessfulOperation, MoveError> MoveResult;
};

#endif