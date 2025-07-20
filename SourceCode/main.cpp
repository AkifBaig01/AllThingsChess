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

    //all_attack_tables();

    //for (int i = 0; i < 64; i++) print_bitboard(rook_masks[i]);
    // print_bitboard(generate_rook_masks(d5));

    //uint64_t bitboard = 0ULL;

    /*
    set_bit(bitboard, c2);
    set_bit(bitboard, g2);
    set_bit(bitboard, h7);
    set_bit(bitboard, d5);
    */
   //set_bit(bitboard, e4);
   //set_bit(bitboard, g2);
   //set_bit(bitboard, d5);
   //set_bit(bitboard, b1);
    

    //print_bitboard(bitboard);

    //print_bitboard(generate_real_bishop_attacks(bitboard, e4));

    //std::cout << "This is the population count: " << popcount(bitboard) << std::endl;
    //std::cout << "This is the lsb index: " << get_lsb_index(bitboard) << std::endl;
    for (int i = 0; i < 4096; i++) {
        print_bitboard(set_blocker_combo(i, popcount(generate_rook_masks(a1)), generate_rook_masks(a1)));
    }

    return 0;
}