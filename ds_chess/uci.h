#ifndef UCI_H   
#define UCI_H

#include "board.h"

namespace UCI {
    const std::string NAME = "ds_chess";
    const std::string AUTHOR = "Justin and Cora";
    //Forsyth Edwards notation for position: 
    // pieces, player to move, castling rights, en passant, 50 move rule, total ply
    const std::string STARTPOS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    
    void uci_loop();
    void uci_command();
    void isready_command();
    void position_command(
        std::vector<std::string>::iterator begin, 
        std::vector<std::string>::iterator end,
        std::optional<Board::Board> *board
    );
    void go_command();
    void print_command();
};

#endif