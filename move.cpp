#include <tuple>
#include <vector>

#include "move.h"

using namespace Move;

Index Move::Move::coord_to_index(size_t rank, size_t file) {
    return rank * 8 + file;
}

//returns tuple of the form (rank, file)
std::tuple<size_t, size_t> Move::Move::index_to_coord(Index index) {
    return std::make_tuple(index / 8, index % 8);
}

std::vector<Index> Piece::generate_legal_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return;
    }
    std::vector<Index> moves = {};
    Piece piece = board->board[index].value();

    switch (piece.piece_type) {
    case PieceType::Pawn:
        moves = generate_pawn_moves(board, index);
        break;
    case PieceType::Knight:
        moves = generate_knight_moves(board, index);
        break;
    case PieceType::Bishop:
        //TODO
        break;
    case PieceType::Rook:
        //TODO
        break;
    case PieceType::Queen:
        //TODO
        break;
    case PieceType::King:
        //TODO
        break;
    }
    return moves;
}

std::vector<Index> Move::Piece::generate_pawn_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return;
    }
    std::vector<Index> moves = {};
    Piece piece = board->board[index].value();
    bool is_piece_white = piece.color == Color::White;
    int32_t dir = is_piece_white ? UP_OFFSET : DOWN_OFFSET;
    
    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    if (board->en_passant.has_value()) {
        size_t ep_rank, ep_file;
        std::tie(ep_rank, ep_file) = Move::index_to_coord(board->en_passant.value());

        size_t abs_diff_ranks = rank > ep_rank ? rank - ep_rank : ep_rank - rank;
        bool can_en_passant = ep_file == file && abs_diff_ranks == 1;
        if (can_en_passant) {
            int32_t offset = dir 
                + (rank < ep_rank ? LEFT_OFFSET : RIGHT_OFFSET);
            moves.push_back(index + offset);
        }
    }
    size_t offset = index + dir;
    if (!board->board[offset].has_value()) {
        moves.push_back(offset);
    }
    bool can_move_two_spaces = 
        (is_piece_white && rank == 1) 
        || (!is_piece_white && rank == 6);
    size_t offset = index + dir * 2;
    if (can_move_two_spaces && !board->is_piece_at_index(index + dir) && !board->is_piece_at_index(offset)) {
        moves.push_back(offset);
    }
    size_t offset = index + dir + LEFT_OFFSET;
    if (file > 0 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + dir + RIGHT_OFFSET;
    if (file < 7 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
}

std::vector<Index> Move::Piece::generate_knight_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return;
    }
    std::vector<Index> moves = {};
    Piece piece = board->board[index].value();
    bool is_piece_white = piece.color == Color::White;
    int32_t dir = is_piece_white ? UP_OFFSET : DOWN_OFFSET;
    
    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    size_t offset = index + LEFT_OFFSET * 2 + UP_OFFSET;
    if (rank < 7 && file > 1 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + LEFT_OFFSET + UP_OFFSET * 2;
    if (rank < 6 && file > 0 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + RIGHT_OFFSET + UP_OFFSET * 2;
    if (rank < 6 && file < 7 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + RIGHT_OFFSET * 2 + UP_OFFSET;
    if (rank < 7 && file < 6 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + RIGHT_OFFSET * 2 + DOWN_OFFSET;
    if (rank > 0 && file < 6 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + RIGHT_OFFSET + DOWN_OFFSET * 2;
    if (rank > 1 && file < 7 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + LEFT_OFFSET + DOWN_OFFSET * 2;
    if (rank > 1 && file > 0 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + LEFT_OFFSET * 2 + DOWN_OFFSET;
    if (rank > 0 && file > 1 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }

}

Color swap(Color color) {
    switch (color) {
        case Color::White:
            return Color::Black;
        case Color::Black:
            return Color::White;
    }
}