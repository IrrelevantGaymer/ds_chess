//
// Created by river on 5/13/24.
//

#include "move.h"

#include <iostream>
#include <tuple>
#include <vector>

#include "board.h"
#include <stdexcept>

bool Move::Piece::operator==(const Piece& other) const {
    return this->color == other.color && this->piece_type == other.piece_type;
}

std::string Move::Piece::to_string() const {
    auto fn = this->color == Color::White ?
    [](char e) -> char {
        return toupper(e);
    }
    : [](char e) -> char {
        return tolower(e);
    };

    switch (this->piece_type) {
    case PieceType::Pawn:
        return std::string(1, fn('p'));
    case PieceType::Knight:
        return std::string(1, fn('n'));
    case PieceType::Bishop:
        return std::string(1, fn('b'));
    case PieceType::Rook:
        return std::string(1, fn('r'));
    case PieceType::Queen:
        return std::string(1, fn('q'));
    case PieceType::King:
        return std::string(1, fn('k'));
    default:
        throw std::exception();
    }
}

std::string Move::Move::to_string() const {
    size_t start_rank, start_file, end_rank, end_file;
    std::tie(start_rank, start_file) = index_to_coord(this->from);
    std::tie(end_rank, end_file) = index_to_coord(this->to);
    std::string output = {(char)(start_file + 'a'), (char)(start_rank + '1'), (char)(end_file + 'a'), (char)(end_rank + '1')};
    return output;
}


Move::Index Move::Move::coord_to_index(size_t rank, size_t file) {
    return rank * 8 + file;
}

//returns tuple of the form (rank, file)
std::tuple<size_t, size_t> Move::Move::index_to_coord(Index index) {
    return std::make_tuple(index / 8, index % 8);
}

Move::Index Move::Move::string_to_index(std::string pos) {
    if (
        pos.length() != 2
        || !isalpha(tolower(pos[0])) || (tolower(pos[0]) < 'a' && tolower(pos[0]) > 'h')
        || !isdigit(pos[1]) || (pos[1] < '1' && pos[1] > '8')
    ) {
        throw std::invalid_argument(
            "expected a string with a file indicated by a letter a-h, then a rank indicated by a number 1-8"
        );
    }

    size_t rank, file;
    rank = pos[1] - '1';
    file = tolower(pos[0]) - 'a';
    return Move::coord_to_index(rank, file);
}

Move::Move Move::Move::string_to_move(std::string move) {
    if (move.length() != 4) {
        throw std::invalid_argument(
            "expected a string such as e2e4"
        );
    }
    Index from = string_to_index(move.substr(0, 2));
    Index to = string_to_index(move.substr(2, 2));

    return Move(from, to, std::nullopt, std::nullopt);
}

std::vector<Move::Index> Move::Piece::generate_legal_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return std::vector<Index>();
    }
    std::vector<Index> moves = std::vector<Index>();
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

std::vector<Move::Index> Move::Piece::generate_capture_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return std::vector<Index>();
    }
    std::vector<Index> moves = std::vector<Index>();
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

std::vector<Move::Index> Move::Piece::generate_pawn_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return std::vector<Index>();
    }
    std::vector<Index> moves = std::vector<Index>();
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
    offset = index + dir * 2;
    if (can_move_two_spaces && !board->is_piece_at_index(index + dir) && !board->is_piece_at_index(offset)) {
        moves.push_back(offset);
    }

    return moves;
}

std::vector<Move::Index> Move::Piece::generate_pawn_capture_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return std::vector<Index>();
    }
    std::vector<Index> moves = std::vector<Index>();
    Piece piece = board->board[index].value();
    bool is_piece_white = piece.color == Color::White;
    int32_t dir = is_piece_white ? UP_OFFSET : DOWN_OFFSET;

    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    size_t offset = index + dir + LEFT_OFFSET;
    if (file > 0 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + dir + RIGHT_OFFSET;
    if (file < 7 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }

    return moves;
}

std::vector<Move::Index> Move::Piece::generate_knight_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return std::vector<Index>();
    }
    std::vector<Index> moves = std::vector<Index>();
    Piece piece = board->board[index].value();
    bool is_piece_white = piece.color == Color::White;
    int32_t dir = is_piece_white ? UP_OFFSET : DOWN_OFFSET;

    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    size_t offset = index + LEFT_OFFSET * 2 + UP_OFFSET;
    if (rank < 7 && file > 1 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + LEFT_OFFSET + UP_OFFSET * 2;
    if (rank < 6 && file > 0 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + RIGHT_OFFSET + UP_OFFSET * 2;
    if (rank < 6 && file < 7 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + RIGHT_OFFSET * 2 + UP_OFFSET;
    if (rank < 7 && file < 6 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + RIGHT_OFFSET * 2 + DOWN_OFFSET;
    if (rank > 0 && file < 6 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + RIGHT_OFFSET + DOWN_OFFSET * 2;
    if (rank > 1 && file < 7 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + LEFT_OFFSET + DOWN_OFFSET * 2;
    if (rank > 1 && file > 0 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + LEFT_OFFSET * 2 + DOWN_OFFSET;
    if (rank > 0 && file > 1 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }

    return moves;
}

std::vector<Move::Index> Move::Piece::generate_knight_capture_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return std::vector<Index>();
    }
    std::vector<Index> moves = std::vector<Index>();
    Piece piece = board->board[index].value();
    bool is_piece_white = piece.color == Color::White;
    int32_t dir = is_piece_white ? UP_OFFSET : DOWN_OFFSET;

    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    size_t offset = index + LEFT_OFFSET * 2 + UP_OFFSET;
    if (rank < 7 && file > 1 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + LEFT_OFFSET + UP_OFFSET * 2;
    if (rank < 6 && file > 0 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + RIGHT_OFFSET + UP_OFFSET * 2;
    if (rank < 6 && file < 7 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + RIGHT_OFFSET * 2 + UP_OFFSET;
    if (rank < 7 && file < 6 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + RIGHT_OFFSET * 2 + DOWN_OFFSET;
    if (rank > 0 && file < 6 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + RIGHT_OFFSET + DOWN_OFFSET * 2;
    if (rank > 1 && file < 7 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + LEFT_OFFSET + DOWN_OFFSET * 2;
    if (rank > 1 && file > 0 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + LEFT_OFFSET * 2 + DOWN_OFFSET;
    if (rank > 0 && file > 1 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }

    return moves;
}

std::vector<Move::Index> Move::Piece::generate_bishop_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return std::vector<Index>();
    }
    std::vector<Index> moves = std::vector<Index>();
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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

std::vector<Move::Index> Move::Piece::generate_bishop_capture_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return std::vector<Index>();
    }
    std::vector<Index> moves = std::vector<Index>();
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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

std::vector<Move::Index> Move::Piece::generate_rook_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return std::vector<Index>();
    }
    std::vector<Index> moves = std::vector<Index>();
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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

std::vector<Move::Index> Move::Piece::generate_rook_capture_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return std::vector<Index>();
    }
    std::vector<Index> moves = std::vector<Index>();
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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

std::vector<Move::Index> Move::Piece::generate_queen_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return std::vector<Index>();
    }
    std::vector<Index> moves = std::vector<Index>();
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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

std::vector<Move::Index> Move::Piece::generate_queen_capture_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return std::vector<Index>();
    }
    std::vector<Index> moves = std::vector<Index>();
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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
    i = index;
    r = rank;
    f = file;
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

std::vector<Move::Index> Move::Piece::generate_king_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return std::vector<Index>();
    }
    std::vector<Index> moves = std::vector<Index>();
    Piece piece = board->board[index].value();

    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    size_t offset = index + LEFT_OFFSET;
    if (file > 0 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + UPPER_LEFT_OFFSET;
    if (rank < 7 && file < 0 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + UP_OFFSET;
    if (rank > 7 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + UPPER_RIGHT_OFFSET;
    if (rank < 7 && file < 7 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + RIGHT_OFFSET;
    if (file < 7 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + LOWER_RIGHT_OFFSET;
    if (rank > 0 && file < 7 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + DOWN_OFFSET;
    if (rank > 0 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + LOWER_LEFT_OFFSET;
    if (rank > 0 && file > 0 && board->can_piece_move_to_square(offset, piece.color)) {
        moves.push_back(offset);
    }

    Index rook_index = Board::Board::get_default_queenside_rook_for_color(piece.color);
    if (
        board->get_queenside_castle_for_color(piece.color)
        && board->board[rook_index].has_value()
        && board->board[rook_index].value().color == piece.color
        && board->board[rook_index].value().piece_type == PieceType::Rook
        && !board->board[rook_index + 1].has_value()
        && !board->board[rook_index + 2].has_value()
        && !board->board[rook_index + 3].has_value()
        //TODO check if traversed square is attacked
    ) {
        moves.push_back(Board::Board::get_default_king_for_color(piece.color) - 2);
    }
    rook_index = Board::Board::get_default_kingside_rook_for_color(piece.color);
    //castling
    if (
        board->get_kingside_castle_for_color(piece.color)
        && board->board[rook_index].has_value()
        && board->board[rook_index].value().color <= piece.color
        && board->board[rook_index].value().piece_type == PieceType::Rook
        && !board->board[rook_index - 1].has_value()
        && !board->board[rook_index - 2].has_value()
        //TODO check if traversed square is attacked
    ) {
        moves.push_back(Board::Board::get_default_king_for_color(piece.color) + 2);
    }

    return moves;
}

std::vector<Move::Index> Move::Piece::generate_king_capture_moves(Board::Board *board, Index index) {
    if (!board->board[index].has_value()) {
        return std::vector<Index>();
    }
    std::vector<Index> moves = std::vector<Index>();
    Piece piece = board->board[index].value();

    size_t rank, file;
    std::tie(rank, file) = Move::index_to_coord(index);

    size_t offset = index + LEFT_OFFSET;
    if (file > 0 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + UPPER_LEFT_OFFSET;
    if (rank < 7 && file < 0 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + UP_OFFSET;
    if (rank > 7 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + UPPER_RIGHT_OFFSET;
    if (rank < 7 && file < 7 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + RIGHT_OFFSET;
    if (file < 7 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + LOWER_RIGHT_OFFSET;
    if (rank > 0 && file < 7 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + DOWN_OFFSET;
    if (rank > 0 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }
    offset = index + LOWER_LEFT_OFFSET;
    if (rank > 0 && file > 0 && board->is_piece_capturable(offset, piece.color)) {
        moves.push_back(offset);
    }

    return moves;
}

Move::Color Move::swap(Color color) {
    switch (color) {
        case White:
            return Black;
        case Black:
            return White;
        default:
            throw std::exception();
    }
}

void Move::swap_ptr(Color *color) {
    switch (*color) {
        case White:
            *color = Black;
            break;
        case Black:
            *color = White;
            break;
    }
}

Move::CastlingRights Move::get_castling_rights(
    bool can_kingside_castle, bool can_queenside_castle
) {
    if (can_kingside_castle && can_queenside_castle) {
        return Both;
    } else if (can_kingside_castle) {
        return Kingside;
    } else if (can_queenside_castle) {
        return Queenside;
    }
    return None;
}