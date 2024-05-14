//
// Created by river on 5/13/24.
//

#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"

namespace Search {

    float search(int32_t depth, Board::Board *board);

    void init_search(int32_t depth, Board::Board *board);

};

#endif