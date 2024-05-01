#include "move.h"

namespace Board {
    class Board {
    public:
        std::optional<Move::PieceType> board[64];
        std::optional<size_t> en_passant;
        Move::Color current_player;

        
    private:

    };
}