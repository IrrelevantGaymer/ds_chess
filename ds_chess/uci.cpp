#include <optional>

#include "uci.h"
#include "board.h"
#include "string_handling.h"

const std::string NAME = "ds_chess";
const std::string AUTHOR = "Justin and Cora";
//Forsyth Edwards notation for position: pieces, player to move, castling rights, en passant, 50 move rule, total ply
const std::string STARTPOS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

void UCI::uci_loop() {
    std::optional<Board::Board> board;

    while (true) {
        std::string input;
        getline(std::cin, input);
        std::vector<std::string> args = StringHandling::split(input, ' ');

        if (args[0] == "uci") {
            UCI::uci_command();
        }//end if
        else if (args[0] == "isready") {
            UCI::isready_command();
        }
        else if (args[0] == "position") {
            size_t i = 1;
            //do position logic
            // check if 
            if (args[i] == "startpos") {
                //set board to startpos
                i += 1;
            }
            else if (args[i] == "fen") {
                //create a board using args[2]
                std::string fen = args[2] + " " + args[3] + " " + args[4] + " " + args[5] + " " + args[6] + " " + args[7];
                board = Board::Board(fen);
                i += 6;
            }

            if (i < args.size() && args[i] == "moves") {
                i += 1;
                while (i < args.size()) {
                    auto move = Move::Move::string_to_move(args[i]);
                    if (board.has_value() && std::holds_alternative<Board::SuccessfulOperation>(
                        board.value().is_valid_move(&move)
                    )) {
                        board.value().make_move(&move);
                    } else {
                        continue;
                    }
                }
            }
        }//end else if "position"

        else if (args[0] == "go") {
            //go will run search function using chessboard as an input

        }
        else if (args[0] == "quit") {
            break;
        }
    } //end while
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
    Board::Board *board
) {
    auto index = begin;
    
    if (*index == "startpos") {
        index += 1;
    } else if (*index == "fen") {
        //create a board using args[2]
        std::string fen = index[1] + " " + index[2] + " " + index[3] + " " + index[4] + " " + index[5] + " " + index[6];
        *board = Board::Board(fen);
        index += 6;
    }

    if (index <= end && *index == "moves") {
        index += 1;
        while (index <= end) {
            auto move = Move::Move::string_to_move(*index);
            if (std::holds_alternative<Board::SuccessfulOperation>(
                board->is_valid_move(&move)
            )) {
                board->make_move(&move);
            } else {
                continue;
            }
        }
    }
}