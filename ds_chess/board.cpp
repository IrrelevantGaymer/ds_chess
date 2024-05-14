//
// Created by river on 5/13/24.
//

#include "board.h"

#include <iostream>
#include <optional>

#include <algorithm>

#include "move.h"

Board::Board::Board (std::string fen) :
    board(std::array<std::optional<Move::Piece>, 64>()),
    current_player(Move::Color::White),
    can_white_kingside_castle(false),
    can_white_queenside_castle(false),
    can_black_kingside_castle(false),
    can_black_queenside_castle(false),
    en_passant(std::nullopt),
    moves_since_last_pawn_move_or_capture(0),
    num_moves(0)
{
    //handle pieces on the board
    Move::Index index = 63;

    size_t i = 0;
    while (index < 64) {
        switch (fen[i]) {
        case 'P':
            this->board[index] = Move::Piece(Move::Color::White, Move::PieceType::Pawn);
            index -= 1;
            break;
        case 'p':
            this->board[index] = Move::Piece(Move::Color::Black, Move::PieceType::Pawn);
            index -= 1;
            break;
        case 'N':
            this->board[index] = Move::Piece(Move::Color::White, Move::PieceType::Knight);
            index -= 1;
            break;
        case 'n':
            this->board[index] = Move::Piece(Move::Color::Black, Move::PieceType::Knight);
            index -= 1;
            break;
        case 'B':
            this->board[index] = Move::Piece(Move::Color::White, Move::PieceType::Bishop);
            index -= 1;
            break;
        case 'b':
            this->board[index] = Move::Piece(Move::Color::Black, Move::PieceType::Bishop);
            index -= 1;
            break;
        case 'R':
            this->board[index] = Move::Piece(Move::Color::White, Move::PieceType::Rook);
            index -= 1;
            break;
        case 'r':
            this->board[index] = Move::Piece(Move::Color::Black, Move::PieceType::Rook);
            index -= 1;
            break;
        case 'Q':
            this->board[index] = Move::Piece(Move::Color::White, Move::PieceType::Queen);
            index -= 1;
            break;
        case 'q':
            this->board[index] = Move::Piece(Move::Color::Black, Move::PieceType::Queen);
            index -= 1;
            break;
        case 'K':
            this->board[index] = Move::Piece(Move::Color::White, Move::PieceType::King);
            index -= 1;
            break;
        case 'k':
            this->board[index] = Move::Piece(Move::Color::Black, Move::PieceType::King);
            index -= 1;
            break;
        case '/':
            break;
        default:
            if (isdigit(fen[i])) {
                if (fen[i] >= '1' && fen[i] <= '8') {
                    index -= fen[i] - '0';
                    i++;
                    continue;
                }
            }
            throw std::invalid_argument("Unexpected value in fen string: " + std::to_string(fen[i]) + " at " + std::to_string(i));
        }
        i++;
    }

    //account for space
    i++;

    //handle whose turn it is
    switch (fen[i]) {
    case 'w':
        this->current_player = Move::Color::White;
        i++;
        break;
    case 'b':
        this->current_player = Move::Color::Black;
        i++;
        break;
    default:
        throw std::invalid_argument("Unexpected value in fen string: " + std::to_string(i) + " " + fen[i] + " " + fen[i - 1] + "\\");
    }

    //acount for space
    i++;

    //handle castling
    while(true) {
        switch (fen[i]) {
        case 'K':
            this->can_white_kingside_castle = true;
            i++;
            break;
        case 'Q':
            this->can_white_queenside_castle = true;
            i++;
            break;
        case 'k':
            this->can_black_kingside_castle = true;
            i++;
            break;
        case 'q':
            this->can_black_queenside_castle = true;
            i++;
            break;
        case '-':
            i++;
            goto exit_castling_loop;
        case ' ':
            goto exit_castling_loop;
        }
    }

    exit_castling_loop: ;

    //acount for space
    i++;

    //handling en passant square
    if (fen[i] != '-') {
        size_t en_passant_index = Move::Move::string_to_index(fen.substr(i, 2));
        this->en_passant = {en_passant_index};
        i += 2;
    } else {
        i++;
    }

    //account for space
    i++;

    //handle moves since last pawn move or capture
    size_t len = 0;
    while (true) {
        if (i + len >= fen.length()) {
            throw std::invalid_argument("expected number for total moves");
        } else if (fen[i + len] == ' ') {
            this->moves_since_last_pawn_move_or_capture = atoi(fen.substr(i, len).c_str());
            i += len;
            break;
        }
        len += 1;
    }

    //acount for space
    i++;

    //handle moves since last pawn move or capture
    len = 0;
    while (true) {
        if (i + len >= fen.length() || fen[i + len] == ' ') {
            this->num_moves = atoi(fen.substr(i, len).c_str());
            i += len;
            break;
        }
        len += 1;
    }
}

bool Board::Board::is_piece_at_index(Move::Index index) const {
    return this->board[index].has_value();
}

bool Board::Board::is_piece_capturable(Move::Index index, Move::Color capturing_color) const {
    if (this->is_piece_at_index(index)) {
        return this->board[index].value().color != capturing_color;
    }
    return false;
}

bool Board::Board::can_piece_move_to_square(Move::Index index, Move::Color capturing_color) const {
    if (this->is_piece_at_index(index)) {
        return this->board[index].value().color != capturing_color;
    }
    return true;
}

std::optional<Move::Index> Board::Board::get_king_index(Move::Color color) const {
    auto it = std::find_if(
        this->board.begin(),
        this->board.end(),
        [color](auto e) {return e.has_value() && e.value() == Move::Piece(color, Move::PieceType::King);}
    );
    if (it != this->board.end()) {
        return std::optional(std::distance(this->board.begin(), it));
    }
    return std::nullopt;
}

bool Board::Board::get_queenside_castle_for_color(Move::Color color) const {
    if (color == Move::Color::White) {
        return this->can_black_kingside_castle;
    }
    return this->can_black_queenside_castle;
}

bool Board::Board::get_kingside_castle_for_color(Move::Color color) const {
    if (color == Move::Color::White) {
        return this->can_white_kingside_castle;
    }
    return this->can_white_queenside_castle;
}

Move::Index Board::Board::get_default_queenside_rook_for_color(Move::Color color) {
    if (color == Move::Color::White) {
        return DEFAULT_WHITE_QUEENSIDE_ROOK_INDEX;
    }
    return DEFAULT_BLACK_QUEENSIDE_ROOK_INDEX;
}

Move::Index Board::Board::get_default_king_for_color(Move::Color color) {
    if (color == Move::Color::White) {
        return DEFAULT_WHITE_KING_INDEX;
    }
    return DEFAULT_BLACK_KING_INDEX;
}

Move::Index Board::Board::get_default_kingside_rook_for_color(Move::Color color) {
    if (color == Move::Color::White) {
        return DEFAULT_WHITE_KINGSIDE_ROOK_INDEX;
    }
    return DEFAULT_BLACK_KINGSIDE_ROOK_INDEX;
}

Board::MoveResult Board::Board::is_valid_move(Move::Move *move) {
    if (!this->board[move->from].has_value()) {
        return MoveResult(MoveError::NoPieceToMove);
    }

    if (move->from >= 64 || move->to >= 64) {
        return MoveResult(MoveError::IndexOutOfBounds);
    }

    Move::Piece piece = this->board[move->from].value();
    auto possible_moves = Move::Piece::generate_legal_moves(this, move->from);

    if (std::find(possible_moves.begin(), possible_moves.end(), move->to) == possible_moves.end()) {
        return MoveResult(MoveError::InvalidMove);
    } else if (piece.color != this->current_player) {
        return MoveResult(MoveError::InvalidMove);
    }

    move->capture = this->board[move->to];
    this->board[move->to] = this->board[move->from];
    this->board[move->from] = std::nullopt;

    Move::swap_ptr(&this->current_player);

    auto king_index = this->get_king_index(piece.color);
    if (!king_index.has_value()) {
        return MoveResult(MoveError::NoKing);
    }
    auto potential_captures = Move::Piece::generate_capture_moves(this, piece.color);
    if (std::find(potential_captures.begin(), potential_captures.end(), king_index.value()) != potential_captures.end()) {
        return MoveResult(MoveError::KingLeftInCheck);
    }

    this->board[move->from] = this->board[move->to];
    this->board[move->to] = move->capture;
    Move::swap_ptr(&this->current_player);

    return MoveResult(SuccessfulOperation {});
}

void Board::Board::make_move(Move::Move *move) {
    Move::Piece moving_piece = this->board[move->from].value();
    bool moved_two_squares_forward = (move->from < move->to ? move->to - move->from : move->from - move->to) == 16;
    bool was_piece_pawn = moving_piece.piece_type == Move::PieceType::Pawn;
    if (moved_two_squares_forward && was_piece_pawn) {
        this->en_passant = std::optional(move->to);
    } else {
        this->en_passant = std::nullopt;
    }

    move->capture = this->board[move->to];

    //handle castling rights
    Move::Index default_king_space = moving_piece.color == Move::Color::White ?
        DEFAULT_WHITE_KING_INDEX
        : DEFAULT_BLACK_KING_INDEX;
    if (moving_piece.piece_type == Move::PieceType::King && default_king_space == move->from) {
        move->lost_castling_rights = Move::get_castling_rights(
            this->get_kingside_castle_for_color(moving_piece.color),
            this->get_queenside_castle_for_color(moving_piece.color)
        );

        if (moving_piece.color == Move::Color::White) {
            this->can_white_kingside_castle = false;
            this->can_white_queenside_castle = false;
        } else {
            this->can_black_kingside_castle = false;
            this->can_black_queenside_castle = false;
        }
    } else if (moving_piece.piece_type == Move::PieceType::Rook) {
        if (moving_piece.color == Move::Color::White) {
            if (move->from == DEFAULT_WHITE_QUEENSIDE_ROOK_INDEX && this->can_white_queenside_castle) {
                this->can_white_queenside_castle = false;
                move->lost_castling_rights = Move::CastlingRights::Queenside;
            } else if (move->from == DEFAULT_WHITE_KINGSIDE_ROOK_INDEX && this->can_white_kingside_castle) {
                this->can_white_kingside_castle = false;
                move->lost_castling_rights = Move::CastlingRights::Kingside;
            }
        } else {
            if (move->from == DEFAULT_BLACK_QUEENSIDE_ROOK_INDEX && this->can_black_queenside_castle) {
                this->can_black_queenside_castle = false;
                move->lost_castling_rights = Move::CastlingRights::Queenside;
            } else if (move->from == DEFAULT_BLACK_KINGSIDE_ROOK_INDEX && this->can_black_kingside_castle) {
                this->can_black_kingside_castle = false;
                move->lost_castling_rights = Move::CastlingRights::Kingside;
            }
        }
    }

    if (move->capture.has_value() || was_piece_pawn) {
        this->moves_since_last_pawn_move_or_capture = 0;
    } else {
        this->moves_since_last_pawn_move_or_capture += 1;
    }

    this->board[move->to] = this->board[move->from];
    this->board[move->from] = std::nullopt;

    //handle castling
    if (moving_piece.piece_type == Move::PieceType::King && move->from == Board::get_default_king_for_color(moving_piece.color)) {
        if (move->to - move->from == 2) {
            this->board[Board::get_default_kingside_rook_for_color(moving_piece.color)] = std::nullopt;
            this->board[Board::get_default_king_for_color(moving_piece.color) + 1] = {
                Move::Piece(moving_piece.color, Move::PieceType::Rook)
            };
            move->lost_castling_rights = Move::CastlingRights::Both;
            if (moving_piece.color == Move::Color::White) {
                this->can_white_kingside_castle = false;
                this->can_white_queenside_castle = false;
            } else {
                this->can_black_kingside_castle = false;
                this->can_black_queenside_castle = false;
            }
        } else if (move->from - move->to == 2) {
            this->board[Board::get_default_queenside_rook_for_color(moving_piece.color)] = std::nullopt;
            this->board[Board::get_default_king_for_color(moving_piece.color) - 1] = {
                Move::Piece(moving_piece.color, Move::PieceType::Rook)
            };
            move->lost_castling_rights = Move::CastlingRights::Both;
            if (moving_piece.color == Move::Color::White) {
                this->can_white_kingside_castle = false;
                this->can_white_queenside_castle = false;
            } else {
                this->can_black_kingside_castle = false;
                this->can_black_queenside_castle = false;
            }
        }
    }
    if (this->current_player == Move::Color::Black) {
        this->num_moves += 1;
    }
    Move::swap_ptr(&this->current_player);
}

void Board::Board::unmake_move(Move::Move *move) {
    Move::Piece moving_piece = this->board[move->to].value();

    this->board[move->from] = this->board[move->to];
    this->board[move->to] = move->capture;
    this->current_player = Move::swap(this->current_player);

    //handling undoing castling
    if (moving_piece.piece_type == Move::PieceType::King && move->from == Board::get_default_king_for_color(moving_piece.color)) {
        if (move->to - move->from == 2) {
            this->board[Board::get_default_kingside_rook_for_color(moving_piece.color)] = {
                Move::Piece(moving_piece.color, Move::PieceType::Rook)
            };
            this->board[Board::get_default_king_for_color(moving_piece.color) + 1] = std::nullopt;
        } else if (move->from - move->to == 2) {
            this->board[Board::get_default_queenside_rook_for_color(moving_piece.color)] = {
                Move::Piece(moving_piece.color, Move::PieceType::Rook)
            };
            this->board[Board::get_default_king_for_color(moving_piece.color) - 1] = std::nullopt;
        }
    }

    //handling regenerating castling rights
    if (moving_piece.color == Move::Color::White) {
        if (move->lost_castling_rights == Move::CastlingRights::Both) {
            this->can_white_queenside_castle = true;
            this->can_white_kingside_castle = true;
        } else if (move->lost_castling_rights == Move::CastlingRights::Queenside) {
            this->can_white_queenside_castle = true;
        } else if (move->lost_castling_rights == Move::CastlingRights::Kingside) {
            this->can_white_kingside_castle = true;
        }
    } else {
        if (move->lost_castling_rights == Move::CastlingRights::Both) {
            this->can_black_queenside_castle = true;
            this->can_black_kingside_castle = true;
        } else if (move->lost_castling_rights == Move::CastlingRights::Queenside) {
            this->can_black_queenside_castle = true;
        } else if (move->lost_castling_rights == Move::CastlingRights::Kingside) {
            this->can_black_kingside_castle = true;
        }
    }
}

void Board::Board::print_board() const {
    for (int32_t rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << " ";
        for (int32_t file = 0; file < 8; file++) {
            Move::Index index = Move::Move::coord_to_index(rank, file);
            std::optional<Move::Piece> piece = this->board[index];

            if (piece.has_value()) {
                std::cout << piece.value().to_string() << " ";
            } else {
                std::cout << "- ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "  a b c d e f g h " << std::endl;
}