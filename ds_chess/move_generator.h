//
// Created by river on 5/13/24.
//

#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <vector>

#include "board.h"
#include "move.h"

namespace MoveGenerator {
    std::vector<Move::Move> generate_moves(Board::Board *board);
    std::vector<Move::Move> generate_capture_moves(Board::Board *board);
};

#endif