#include <tuple>
#include <vector>

#include "move.h"

using namespace Move;

std::string Move::Piece::to_string() const {
    auto fn = this->color == Color::White ? 
    [](char e) { 
        return toupper(e); 
    } 
    : [](char e) { 
        return tolower(e); 
    };

    switch (this->piece_type) {
    case PieceType::Pawn:
        return std::string(fn('p'), 1);
    case PieceType::Knight:
        return std::string(fn('n'), 1);
    case PieceType::Bishop:
        return std::string(fn('b'), 1);
    case PieceType::Rook:
        return std::string(fn('r'), 1);
    case PieceType::Queen:
        return std::string(fn('q'), 1);
    case PieceType::King:
        return std::string(fn('k'), 1);
    }
}

Index Move::Move::coord_to_index(size_t rank, size_t file) {
    return rank * 8 + file;
}

//returns tuple of the form (rank, file)
std::tuple<size_t, size_t> Move::Move::index_to_coord(Index index) {
    return std::make_tuple(index / 8, index % 8);
}

std::vector<Index> Piece::generate_legal_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return {};
    }
    std::vector<Index> moves = {};
    Piece piece = board->board[index].value();

    switch (piece.piece_type) {
    case PieceType::Pawn: {
        moves = generate_pawn_moves(board, index);
        auto capture_moves = generate_pawn_capture_moves(board, index);
        moves.insert(moves.end(), capture_moves.begin(), capture_moves.end());
        break;
    }
    case PieceType::Knight:
        moves = generate_knight_moves(board, index);
        break;
    case PieceType::Bishop:
        moves = generate_bishop_moves(board, index);
        break;
    case PieceType::Rook:
        moves = generate_rook_moves(board, index);
        break;
    case PieceType::Queen:
        moves = generate_queen_moves(board, index);
        break;
    case PieceType::King:
        moves = generate_king_moves(board, index);
        break;
    }
    return moves;
}

std::vector<Index> Move::Piece::generate_capture_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return {};
    }
    std::vector<Index> moves = {};
    Piece piece = board->board[index].value();

    switch (piece.piece_type) {
    case PieceType::Pawn: 
        moves = generate_pawn_capture_moves(board, index);
        break;
    case PieceType::Knight:
        moves = generate_knight_capture_moves(board, index);
        break;
    case PieceType::Bishop:
        moves = generate_bishop_capture_moves(board, index);
        break;
    case PieceType::Rook:
        moves = generate_rook_capture_moves(board, index);
        break;
    case PieceType::Queen:
        moves = generate_queen_capture_moves(board, index);
        break;
    case PieceType::King:
        moves = generate_king_capture_moves(board, index);
        break;
    }
    return moves;
}

std::vector<Index> Move::Piece::generate_pawn_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return {};
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

    return moves;
}

std::vector<Index> Move::Piece::generate_pawn_capture_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return {};
    }
    std::vector<Index> moves = {};
    Piece piece = board->board[index].value();
    bool is_piece_white = piece.color == Color::White;
    int32_t dir = is_piece_white ? UP_OFFSET : DOWN_OFFSET;
    
    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    size_t offset = index + dir + LEFT_OFFSET;
    if (file > 0 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + dir + RIGHT_OFFSET;
    if (file < 7 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }

    return moves;
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

    return moves;
}

std::vector<Index> Move::Piece::generate_knight_capture_moves(Board::Board *board, Index index) {
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
    if (rank < 7 && file > 1 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + LEFT_OFFSET + UP_OFFSET * 2;
    if (rank < 6 && file > 0 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + RIGHT_OFFSET + UP_OFFSET * 2;
    if (rank < 6 && file < 7 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + RIGHT_OFFSET * 2 + UP_OFFSET;
    if (rank < 7 && file < 6 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + RIGHT_OFFSET * 2 + DOWN_OFFSET;
    if (rank > 0 && file < 6 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + RIGHT_OFFSET + DOWN_OFFSET * 2;
    if (rank > 1 && file < 7 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + LEFT_OFFSET + DOWN_OFFSET * 2;
    if (rank > 1 && file > 0 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + LEFT_OFFSET * 2 + DOWN_OFFSET;
    if (rank > 0 && file > 1 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }

    return moves;
}

std::vector<Index> Move::Piece::generate_bishop_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return;
    }
    std::vector<Index> moves = {};
    Piece piece = board->board[index].value();
    
    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f > 0 && r < 7 && board->can_piece_move_to_square(i + UPPER_LEFT_OFFSET, piece.color)) {
            i += UPPER_LEFT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f < 7 && r < 7 && board->can_piece_move_to_square(i + UPPER_RIGHT_OFFSET, piece.color)) {
            i += UPPER_RIGHT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f < 7 && r > 0 && board->can_piece_move_to_square(i + LOWER_RIGHT_OFFSET, piece.color)) {
            i += LOWER_RIGHT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f > 0 && r > 0 && board->can_piece_move_to_square(i + LOWER_LEFT_OFFSET, piece.color)) {
            std::tie(r, f) = Move::index_to_coord(i);
            i += LOWER_LEFT_OFFSET;
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }

    return moves;
}

std::vector<Index> Move::Piece::generate_bishop_capture_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return;
    }
    std::vector<Index> moves = {};
    Piece piece = board->board[index].value();
    
    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f > 0 && r < 7 && board->can_piece_move_to_square(i + UPPER_LEFT_OFFSET, piece.color)) {
            i += UPPER_LEFT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f < 7 && r < 7 && board->can_piece_move_to_square(i + UPPER_RIGHT_OFFSET, piece.color)) {
            i += UPPER_RIGHT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f < 7 && r > 0 && board->can_piece_move_to_square(i + LOWER_RIGHT_OFFSET, piece.color)) {
            i += LOWER_RIGHT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f > 0 && r > 0 && board->can_piece_move_to_square(i + LOWER_LEFT_OFFSET, piece.color)) {
            std::tie(r, f) = Move::index_to_coord(i);
            i += LOWER_LEFT_OFFSET;
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }

    return moves;
}

std::vector<Index> Move::Piece::generate_rook_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return;
    }
    std::vector<Index> moves = {};
    Piece piece = board->board[index].value();
    
    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f > 0 && board->can_piece_move_to_square(i + LEFT_OFFSET, piece.color)) {
            i += LEFT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (r < 7 && board->can_piece_move_to_square(i + UP_OFFSET, piece.color)) {
            i += UP_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f < 7 && board->can_piece_move_to_square(i + RIGHT_OFFSET, piece.color)) {
            i += RIGHT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (r > 0 && board->can_piece_move_to_square(i + DOWN_OFFSET, piece.color)) {
            std::tie(r, f) = Move::index_to_coord(i);
            i += LOWER_LEFT_OFFSET;
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }

    return moves;
}

std::vector<Index> Move::Piece::generate_rook_capture_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return;
    }
    std::vector<Index> moves = {};
    Piece piece = board->board[index].value();
    
    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f > 0 && board->can_piece_move_to_square(i + LEFT_OFFSET, piece.color)) {
            i += LEFT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (r < 7 && board->can_piece_move_to_square(i + UP_OFFSET, piece.color)) {
            i += UP_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f < 7 && board->can_piece_move_to_square(i + RIGHT_OFFSET, piece.color)) {
            i += RIGHT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (r > 0 && board->can_piece_move_to_square(i + DOWN_OFFSET, piece.color)) {
            std::tie(r, f) = Move::index_to_coord(i);
            i += LOWER_LEFT_OFFSET;
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }

    return moves;
}

std::vector<Index> Move::Piece::generate_queen_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return;
    }
    std::vector<Index> moves = {};
    Piece piece = board->board[index].value();
    
    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f > 0 && board->can_piece_move_to_square(i + LEFT_OFFSET, piece.color)) {
            i += LEFT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f > 0 && r < 7 && board->can_piece_move_to_square(i + UPPER_LEFT_OFFSET, piece.color)) {
            i += UPPER_LEFT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (r < 7 && board->can_piece_move_to_square(i + UP_OFFSET, piece.color)) {
            i += UP_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f < 7 && r < 7 && board->can_piece_move_to_square(i + UPPER_RIGHT_OFFSET, piece.color)) {
            i += UPPER_RIGHT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f < 7 && board->can_piece_move_to_square(i + RIGHT_OFFSET, piece.color)) {
            i += RIGHT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f < 7 && r > 0 && board->can_piece_move_to_square(i + LOWER_RIGHT_OFFSET, piece.color)) {
            i += LOWER_RIGHT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (r > 0 && board->can_piece_move_to_square(i + DOWN_OFFSET, piece.color)) {
            std::tie(r, f) = Move::index_to_coord(i);
            i += DOWN_OFFSET;
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f > 0 && r > 0 && board->can_piece_move_to_square(i + LOWER_LEFT_OFFSET, piece.color)) {
            std::tie(r, f) = Move::index_to_coord(i);
            i += LOWER_LEFT_OFFSET;
            moves.push_back(i);
            if (board->is_piece_capturable(i, piece.color)) {
                break;
            }
            continue;
        }
        break;
    }

    return moves;
}

std::vector<Index> Move::Piece::generate_queen_capture_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return;
    }
    std::vector<Index> moves = {};
    Piece piece = board->board[index].value();
    
    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f > 0 && board->can_piece_move_to_square(i + LEFT_OFFSET, piece.color)) {
            i += LEFT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f > 0 && r < 7 && board->can_piece_move_to_square(i + UPPER_LEFT_OFFSET, piece.color)) {
            i += UPPER_LEFT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (r < 7 && board->can_piece_move_to_square(i + UP_OFFSET, piece.color)) {
            i += UP_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f < 7 && r < 7 && board->can_piece_move_to_square(i + UPPER_RIGHT_OFFSET, piece.color)) {
            i += UPPER_RIGHT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f < 7 && board->can_piece_move_to_square(i + RIGHT_OFFSET, piece.color)) {
            i += RIGHT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f < 7 && r > 0 && board->can_piece_move_to_square(i + LOWER_RIGHT_OFFSET, piece.color)) {
            i += LOWER_RIGHT_OFFSET;
            std::tie(r, f) = Move::index_to_coord(i);
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (r > 0 && board->can_piece_move_to_square(i + DOWN_OFFSET, piece.color)) {
            std::tie(r, f) = Move::index_to_coord(i);
            i += DOWN_OFFSET;
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }
    size_t i = index;
    size_t r = rank;
    size_t f = file;
    while (true) {
        if (f > 0 && r > 0 && board->can_piece_move_to_square(i + LOWER_LEFT_OFFSET, piece.color)) {
            std::tie(r, f) = Move::index_to_coord(i);
            i += LOWER_LEFT_OFFSET;
            if (board->is_piece_capturable(i, piece.color)) {
                moves.push_back(i);
                break;
            }
            continue;
        }
        break;
    }

    return moves;
}

std::vector<Index> Move::Piece::generate_king_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return;
    }
    std::vector<Index> moves = {};
    Piece piece = board->board[index].value();
    
    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    size_t offset = index + LEFT_OFFSET;
    if (file > 0 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + UPPER_LEFT_OFFSET;
    if (rank < 7 && file < 0 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + UP_OFFSET;
    if (rank > 7 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + UPPER_RIGHT_OFFSET;
    if (rank < 7 && file < 7 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + RIGHT_OFFSET;
    if (file < 7 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + LOWER_RIGHT_OFFSET;
    if (rank > 0 && file < 7 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + DOWN_OFFSET;
    if (rank > 0 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + LOWER_LEFT_OFFSET;
    if (rank > 0 && file > 0 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }

    return moves;
}

std::vector<Index> Move::Piece::generate_king_capture_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return;
    }
    std::vector<Index> moves = {};
    Piece piece = board->board[index].value();
    
    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    size_t offset = index + LEFT_OFFSET;
    if (file > 0 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + UPPER_LEFT_OFFSET;
    if (rank < 7 && file < 0 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + UP_OFFSET;
    if (rank > 7 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + UPPER_RIGHT_OFFSET;
    if (rank < 7 && file < 7 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + RIGHT_OFFSET;
    if (file < 7 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + LOWER_RIGHT_OFFSET;
    if (rank > 0 && file < 7 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + DOWN_OFFSET;
    if (rank > 0 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    size_t offset = index + LOWER_LEFT_OFFSET;
    if (rank > 0 && file > 0 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }

    return moves;
}

Color swap(Color color) {
    switch (color) {
        case Color::White:
            return Color::Black;
        case Color::Black:
            return Color::White;
    }
}

void swap_ptr(Color *color) {
    switch (*color) {
        case Color::White:
            *color = Color::Black;
            break;
        case Color::Black:
            *color = Color::White;
            break;
    }
}