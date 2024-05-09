#ifndef UCI_H   
#define UCI_H

#include "board.h"

namespace UCI {
    void uci_loop();
    void uci_command();
    void isready_command();
    void position_command(
        std::vector<std::string>::iterator begin, 
        std::vector<std::string>::iterator end,
        Board::Board *board
    );
    void go_command();
    void print_command();
    void quit_command();
};

#endif