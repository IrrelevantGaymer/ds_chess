//
// Created by river on 5/13/24.
//

#ifndef MOVE_H
#define MOVE_H

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

//#include "board.h"

namespace Board {
    class Board;
}

namespace Move {
    typedef size_t Index;

    const int32_t UPPER_LEFT_OFFSET  = 7;
    const int32_t UP_OFFSET          = 8;
    const int32_t UPPER_RIGHT_OFFSET = 9;
    const int32_t LEFT_OFFSET        = -1;
    const int32_t RIGHT_OFFSET       = 1;
    const int32_t LOWER_LEFT_OFFSET  = -9;
    const int32_t DOWN_OFFSET        = -8;
    const int32_t LOWER_RIGHT_OFFSET = -7;

    enum Color {
        White, Black
    };

    enum PieceType {
        Pawn, Knight, Bishop, Rook, Queen, King
    };

    enum CastlingRights {
        None, Kingside, Queenside, Both
    };
    class Piece {
    public:
        Color color;
        PieceType piece_type;

        Piece(Color color, PieceType piece_type) : color(color), piece_type(piece_type) {}
        bool operator==(const Piece& other) const;
        std::string to_string() const;
        static std::vector<Index> generate_legal_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_capture_moves(Board::Board *board, Index index);
    private:
        static std::vector<Index> generate_pawn_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_pawn_capture_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_knight_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_knight_capture_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_bishop_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_bishop_capture_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_rook_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_rook_capture_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_queen_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_queen_capture_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_king_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_king_capture_moves(Board::Board *board, Index index);
    };

    class Move {
    public:
        Index from;
        Index to;
        std::optional<Piece> capture;
        std::optional<Piece> promotion;
        CastlingRights lost_castling_rights;

        Move(Index from, Index to, std::optional<Piece> capture, std::optional<Piece> promotion)
            : from(from), to(to), capture(capture), promotion(promotion) {}
        std::string to_string() const;
        static Index coord_to_index(size_t rank, size_t file);
        //returns tuple of the form (rank, file)
        static std::tuple<size_t, size_t> index_to_coord(Index index);
        static Index string_to_index(std::string pos);
        static Move string_to_move(std::string move);
    };

    Color swap(Color color);
    void swap_ptr(Color *color);

    CastlingRights get_castling_rights(
        bool can_kingside_castle, bool can_queenside_castle
    );
};

#endif