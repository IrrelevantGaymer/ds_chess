//
// Created by river on 5/13/24.
//

#include "evaluation.h"

#include <numeric>
#include <ranges>

float get_piece_value(Move::Piece piece) {
    float value;
    switch (piece.piece_type) {
        case Move::PieceType::Pawn:
            value = 1.0;
        break;
        case Move::PieceType::Knight:
            value = 2.9;
        break;
        case Move::PieceType::Bishop:
            value = 3.1;
        break;
        case Move::PieceType::Rook:
            value = 5.0;
        break;
        case Move::PieceType::Queen:
            value = 9.0;
        break;
        case Move::PieceType::King:
            value = 0.0;
        break;
        default:
            return 0.0;
    }

    float modifier = piece.color == Move::Color::White ? 1.0 : -1.0;
    return value * modifier;
}

float evaluate_board(Board::Board *board) {
    auto is_space_occupied_filter = std::views::filter([](std::optional<Move::Piece> piece) {
        return piece.has_value();
    });
    auto unwrap_transform = std::views::transform([](std::optional<Move::Piece> piece) {
        return piece.value();
    });
    auto is_not_king_filter = std::views::filter([](Move::Piece piece) {
        return piece.piece_type != Move::PieceType::King;
    });
    auto get_piece_value_transform = std::views::transform([] (Move::Piece piece) {
        return get_piece_value(piece);
    });

    auto evaluatable_pieces = board->board
        | is_space_occupied_filter
        | unwrap_transform
        | is_not_king_filter
        | get_piece_value_transform;

    return std::accumulate(evaluatable_pieces.begin(), evaluatable_pieces.end(), 0);
}

/*
float evaluate_board(Board::Board *board) {
    float sum = 0;
    for (std::optional<Move::Piece> piece : board->board) {
        if (piece.has_value() && piece.value().piece_type != Move::PieceType::King) {
            sum += get_piece_value(piece.value());
        }
    }
    return sum;
}
*/