#ifndef MOVE_H
#define MOVE_H

#include <cstdint>
#include <optional>
#include <vector>

#include "board.h"
namespace Move {
    typedef size_t Index;

    class Move {
    public:
        Index from;
        Index to;
        std::optional<Piece> capture;
        std::optional<Piece> promotion;

        Move(Index from, Index to, std::optional<Piece> capture, std::optional<Piece> promotion) 
            : from(from), to(to), capture(capture), promotion(promotion) {}
        static Index coord_to_index(size_t rank, size_t file);
        //returns tuple of the form (rank, file)
        static std::tuple<size_t, size_t> index_to_coord(Index index);
    };

    const int32_t UPPER_LEFT_OFFSET  = 7;
    const int32_t UP_OFFSET          = 8;
    const int32_t UPPER_RIGHT_OFFSET = 9;
    const int32_t LEFT_OFFSET        = -1;
    const int32_t RIGHT_OFFSET       = 1;
    const int32_t LOWER_LEFT_OFFSET  = -9;
    const int32_t DOWN_OFFSET        = -8;
    const int32_t LOWER_RIGHT_OFFSET = -7;

    class Piece {
    public:
        Color color;
        PieceType piece_type;

        Piece(Color color, PieceType piece_type) : color(color), piece_type(piece_type) {}
        static std::vector<Index> generate_legal_moves(Board::Board *board, Index index);
        
    private:
        static std::vector<Index> generate_pawn_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_knight_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_bishop_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_rook_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_queen_moves(Board::Board *board, Index index);
        static std::vector<Index> generate_king_moves(Board::Board *board, Index index);
    };

    enum Color {
        White, Black
    };

    enum PieceType {
        Pawn, Knight, Bishop, Rook, Queen, King
    };
}

#endif