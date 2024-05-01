#include <cstdint>
#include <optional>

namespace Move {
    class Move {
    public:
        size_t from;
        size_t to;
        std::optional<PieceType> capture;
    private:

    };

    enum Color {
        White, Black
    };

    enum PieceType {
        Pawn, Knight, Bishop, Rook, Queen, King
    };
}