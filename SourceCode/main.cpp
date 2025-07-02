#include "uci.h"
#include "bitboard.h"
#include "utils.h"

#include <string>


int main() {
    std::string fen = "8/5k2/8/5Pp1/8/6K1/8/8 b - f3 12 63";

    fen_rep board = fen_parser(fen);

    helper_print(board);

    return 0;
}