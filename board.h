#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <tuple>
#include <variant>

#include "move.h"

namespace Board {
    const Move::Index DEFAULT_WHITE_QUEENSIDE_ROOK_INDEX = 0;
    const Move::Index DEFAULT_WHITE_KINGSIDE_ROOK_INDEX = 7;
    const Move::Index DEFAULT_WHITE_KING_INDEX = 4;
    const Move::Index DEFAULT_BLACK_QUEENSIDE_ROOK_INDEX = 56;
    const Move::Index DEFAULT_BLACK_KING_INDEX = 60;
    const Move::Index DEFAULT_BLACK_KINGSIDE_ROOK_INDEX = 63;
    
    class Board {
    public:
        std::array<std::optional<Move::Piece>, 64> board;
        Move::Color current_player;
        bool can_white_kingside_castle, can_white_queenside_castle, can_black_kingside_castle, can_black_queenside_castle;
        std::optional<size_t> en_passant;
        //for fifty-move rule
        size_t moves_since_last_pawn_move_or_capture;
        size_t num_moves;

        Board::Board (std::string fen);
        //returns true if a piece exists at the index
        bool is_piece_at_index(Move::Index index) const;
        //returns true if a piece exists at the index and if the piece is the opposite color to capturing_color
        bool is_piece_capturable(Move::Index index, Move::Color capturing_color) const;
        //returns true if a piece doesn't exist at the index OR if one does, that its color is opposite to capturing_color
        bool can_piece_move_to_square(Move::Index index, Move::Color capturing_color) const;
        //gets index of the king
        std::optional<Move::Index> get_king_index(Move::Color color) const;

        bool get_queenside_castle_for_color(Move::Color color) const;
        bool get_kingside_castle_for_color(Move::Color color) const;
        static Move::Index get_default_queenside_rook_for_color(Move::Color color);
        static Move::Index get_default_king_for_color(Move::Color color);
        static Move::Index get_default_kingside_rook_for_color(Move::Color color);

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