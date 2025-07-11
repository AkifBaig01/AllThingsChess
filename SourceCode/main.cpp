#include "uci.h"
#include "bitboard.h"
#include "utils.h"

// For testing only change later
#include "movegen.cpp"

#include <string>


int main() {
    /*
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    fen_rep board = fen_parser(fen);

    full_pos represent = set_bitboard_pos(board);

    for (int i = 0; i < piece_no; i++){
        print_bitboard(represent.bitboard[i]);
    }

    std::cout << represent.white_king_side_castle << '\n';
    std::cout << represent.white_queen_side_castle << '\n';
    std::cout << represent.black_king_side_castle << '\n';
    std::cout << represent.black_queen_side_castle << '\n';

    std::cout << represent.enpassant_square << '\n';

    std::cout << represent.half_move_clock<< '\n';

    std::cout << represent.full_move_clock << '\n';

    std::cout << represent.to_move << '\n';
    */

    all_attack_tables();

    //print_bitboard(generate_king_attacks(g1));

    for (int i = 0; i < 64; i++) {
        print_bitboard(king_attacks[i]);
    }

    return 0;
}