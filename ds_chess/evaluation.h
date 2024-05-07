#ifndef EVALUATION_H
#define EVALUATION_H

#include "board.h"

namespace Evaluation {

    float evaluate_board(Board::Board *board);
    float get_piece_value(Move::Piece piece);
};

#endif