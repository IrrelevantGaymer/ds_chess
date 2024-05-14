//
// Created by river on 5/13/24.
//
#include <iostream>
#include <optional>

#include "uci.h"
#include "board.h"
#include "search.h"
#include "string_handling.h"

void UCI::uci_loop() {
    std::optional<Board::Board> board;

    while (true) {
        std::string input;
        getline(std::cin, input);
        std::vector<std::string> args = StringHandling::split(input, ' ');

        if (args.empty()) {
            continue;
        }

        if (args[0] == "uci") {
            UCI::uci_command();
        } else if (args[0] == "isready") {
            UCI::isready_command();
        } else if (args[0] == "position") {
            UCI::position_command(args.begin() + 1, args.end(), &board);
        } else if (args[0] == "go") {
            UCI::go_command(args.begin() + 1, args.end(), &board);
        } else if (args[0] == "print") {
            UCI::print_command(&board);
        } else if (args[0] == "quit") {
            break;
        } else {
            std::cout << "invalid command" << std::endl;
        }
    }
}

void UCI::uci_command() {
    std::cout << "id name " << NAME << "\n";
    std::cout << "id author " << AUTHOR << std::endl;
}

void UCI::isready_command() {
    std::cout << "readyok" << std::endl;
}

void UCI::position_command(
    std::vector<std::string>::iterator begin,
    std::vector<std::string>::iterator end,
    std::optional<Board::Board> *board
) {
    auto index = begin;

    if (*index == "startpos") {
        *board = Board::Board(STARTPOS);
        index += 1;
    } else if (*index == "fen") {
        std::string fen = index[1] + " " + index[2] + " " + index[3] + " " + index[4] + " " + index[5] + " " + index[6];
        *board = Board::Board(fen);
        index += 6;
    } else {
        return;
    }

    if (index >= end || *index != "moves") {
        return;
    }

    index += 1;
    while (index < end) {
        auto move = Move::Move::string_to_move(*index);
        if (board->has_value() && std::holds_alternative<Board::SuccessfulOperation>(
            board->value().is_valid_move(&move)
        )) {
            board->value().make_move(&move);
        } else {
            std::cout << "invalid move " << move.to_string() << std::endl;
            auto e = std::get<Board::MoveError>(board->value().is_valid_move(&move));
            std::cout << e << std::endl;
            return;
        }
        index += 1;
    }
}

void UCI::go_command(
    std::vector<std::string>::iterator begin,
    std::vector<std::string>::iterator end,
    std::optional<Board::Board> *board
) {
    auto index = begin;

    int32_t depth = 4;

    while (index < end) {
        if (*index == "depth") {
            index += 1;
            depth = atoi(index->c_str());
        } //check for other go paramters
        index += 1;
    }
    Search::init_search(depth, &board->value());
}

void UCI::print_command(std::optional<Board::Board> *board) {
    if (board->has_value()) {
        board->value().print_board();
    } else {
        std::cout << "no board stored" << std::endl;
    }
}