#include <ranges>

#include "move_generator.h"

std::vector<Move::Move> generate_moves(Board::Board *board) {
    std::vector<Move::Move> moves = {};

    for (Move::Index i = 0; i < std::size(board->board); i++) {
        if (!board->board[i].has_value()) {
            continue;
        }
        Move::Piece piece = board->board[i].value();
        std::vector<Move::Move> moves_for_piece = {};
        size_t last_rank = piece.color == Move::Color::White ? 7 : 0;
        for (Move::Index index_for_piece : Move::Piece::generate_legal_moves(board, i)) {
            if (piece.piece_type == Move::PieceType::Pawn && std::get<1>(Move::Move::index_to_coord(i)) == last_rank) {
                moves_for_piece.push_back(Move::Move(i, index_for_piece, {}, Move::Piece(piece.color, Move::PieceType::Knight)));
                moves_for_piece.push_back(Move::Move(i, index_for_piece, {}, Move::Piece(piece.color, Move::PieceType::Bishop)));
                moves_for_piece.push_back(Move::Move(i, index_for_piece, {}, Move::Piece(piece.color, Move::PieceType::Rook)));
                moves_for_piece.push_back(Move::Move(i, index_for_piece, {}, Move::Piece(piece.color, Move::PieceType::Queen)));
                continue;
            }
            moves_for_piece.push_back(Move::Move(i, index_for_piece, {}, {}));
        }
        auto legal_moves = moves_for_piece | std::ranges::views::filter([board](Move::Move &move) {
            return std::holds_alternative<Board::SuccessfulOperation>(board->is_valid_move(&move));
        });

        moves.insert(moves.end(), legal_moves.begin(), legal_moves.end());
    }

    return moves;
}

std::vector<Move::Move> generate_capture_moves(Board::Board *board) {
    std::vector<Move::Move> moves = {};

    for (Move::Index i = 0; i < std::size(board->board); i++) {
        if (!board->board[i].has_value()) {
            continue;
        }
        Move::Piece piece = board->board[i].value();
        std::vector<Move::Move> moves_for_piece = {};
        size_t last_rank = piece.color == Move::Color::White ? 7 : 0;
        for (Move::Index index_for_piece : Move::Piece::generate_capture_moves(board, i)) {
            if (piece.piece_type == Move::PieceType::Pawn && std::get<1>(Move::Move::index_to_coord(i)) == last_rank) {
                moves_for_piece.push_back(Move::Move(i, index_for_piece, {}, Move::Piece(piece.color, Move::PieceType::Knight)));
                moves_for_piece.push_back(Move::Move(i, index_for_piece, {}, Move::Piece(piece.color, Move::PieceType::Bishop)));
                moves_for_piece.push_back(Move::Move(i, index_for_piece, {}, Move::Piece(piece.color, Move::PieceType::Rook)));
                moves_for_piece.push_back(Move::Move(i, index_for_piece, {}, Move::Piece(piece.color, Move::PieceType::Queen)));
                continue;
            }
            moves_for_piece.push_back(Move::Move(i, index_for_piece, {}, {}));
        }
        auto legal_moves = moves_for_piece | std::ranges::views::filter([board](Move::Move &move) {
            return std::holds_alternative<Board::SuccessfulOperation>(board->is_valid_move(&move));
        });

        moves.insert(moves.end(), legal_moves.begin(), legal_moves.end());
    }

    return moves;
}