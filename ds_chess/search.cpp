//
// Created by river on 5/13/24.
//

#include "search.h"

#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <optional>
#include "move_generator.h"

float Search::search(int32_t depth, Board::Board* board) {

    if (depth == 0) {
        //check for checkmate and/or draw
        return 0.0;
    }

    /*

    have a vector moves = movegenerator::generate_moves
    float bestEval = -numeric_limits::infinity();
    for (Move move : moves) {
        board.make_move(move);
        float eval = search(depth -1, board);
        board.unmake_move(move);
        if eval > bestEval {
            bestEval = eval;
            }
    }
    if (depth == *our beginning depth*
        set output move to bestMove
    use move generator to move
    */
    std::vector<Move::Move> moves = MoveGenerator::generate_moves(board);
    float bestEval = -std::numeric_limits<float>::infinity();
    for (Move::Move move : moves) {
        board->make_move(&move);
        float eval = -search(depth - 1, board);
        board->unmake_move(&move);
        if (eval > bestEval){
            bestEval = eval;
        }
    }

    return bestEval;
}


void Search::init_search(int32_t depth, Board::Board* board) {

    std::optional<Move::Move> best_move = std::nullopt;
    /*
    recursive searching
    best_move
    print it out
    search
    recursive searching
    */
    std::vector<Move::Move> moves = MoveGenerator::generate_moves(board);
    float bestEval = -std::numeric_limits<float>::infinity();
    for (Move::Move move : moves) {
        board->make_move(&move);
        float eval = -search(depth - 1, board);
        board->unmake_move(&move);
        if (eval > bestEval) {
            bestEval = eval;
            best_move = move;
        }
    }

    std::cout << "bestmove " << best_move.value().to_string() << std::endl;
}