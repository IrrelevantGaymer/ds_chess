#include <optional>

#include "uci.h"
#include "board.h"
#include "string_handling.h"

void UCI::uci_loop() {
    std::optional<Board::Board> board;

    while (true) {
        std::string input;
        getline(std::cin, input);
        std::vector<std::string> args = StringHandling::split(input, ' ');

        if (args[0] == "uci") {
            UCI::uci_command();
        } else if (args[0] == "isready") {
            UCI::isready_command();
        } else if (args[0] == "position") {
            UCI::position_command(args.begin() + 1, args.end(), &board);
        } else if (args[0] == "go") {
            UCI::go_command();
            //TODO
        } else if (args[0] == "quit") {
            break;
        }
    }
}

void UCI::uci_command() {
    std::cout << "id name " << NAME << "\n";
    std::cout << "id author" << AUTHOR << std::endl;
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
        index += 1;
    } else if (*index == "fen") {
        std::string fen = index[1] + " " + index[2] + " " + index[3] + " " + index[4] + " " + index[5] + " " + index[6];
        *board = Board::Board(fen);
        index += 6;
    }

    if (index > end || *index != "moves") {
        return;
    }

    index += 1;
    while (index <= end) {
        auto move = Move::Move::string_to_move(*index);
        if (board->has_value() && std::holds_alternative<Board::SuccessfulOperation>(
            board->value().is_valid_move(&move)
        )) {
            board->value().make_move(&move);
        } else {
            return;
        }
    }
}

void UCI::go_command() {
    //TODO
}