
#include "search.h"
#include <iostream>
#include <vector>
#include <string>
#include "move_generator.h"

using namespace MoveGenerator;
using namespace std;

float search(int32_t depth, Board::Board* board) {

	int origdepth = depth;

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
	vector<Move::Move> moves = MoveGenerator::generate_moves(board);
	float bestEval = -numeric_limits::infinity();
	for (Move move : moves) {
		board->make_move(move);
		float eval = -search(depth - 1, board);
		board.unmake_move(move);
		if (eval > bestEval){
			bestEval = eval;
		}
	}
}


void init search(int32_t depth, Board::Board* board) {

	std::optional<Move::move> best_move = std::nullopt;
	/*
	recursive searching
	best_move
	print it out
	search
	recursive searching
	*/
	vector<Move::Move> moves = MoveGenerator::generate_moves(board);
	float bestEval = -numeric_limits::infinity();
	for (Move move : moves) {
		board->make_move(move);
		float eval = -search(depth - 1, board);
		board.unmake_move(move);
		if (eval > bestEval) {
			bestEval = move;
		}
	}
	
}