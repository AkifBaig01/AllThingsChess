#include "uci.h"
#include "bitboard.h"
#include "utils.h"
#include "movegen.h"


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
    //for (int i = 0; i < 4096; i++) {
        //print_bitboard(set_blocker_combo(i, popcount(generate_rook_masks(a1)), generate_rook_masks(a1)));
    //}

    /*
    U64 bishopAttacks(U64 occ, enumSquare sq) {
   blockers = occ & mBishopTbl[sq].mask;
   occ = blockers * mBishopTbl[sq].magic;
   occ >>= 64-relevant_bits; 

   return bishopAttacks[occ]; // no offset
    }
    */
   /*
    all_attack_tables();
    init_rook_attacks();
    uint64_t blockers = 0ULL;
    set_bit(blockers, b4);
    set_bit(blockers, e7);
    set_bit(blockers, e2);
    set_bit(blockers, g4);
    //uint64_t occ = blockers * rook_magics[a1];
    //occ >>= 64 - popcount(rook_masks[a1]);

    //print_bitboard(rook_masks[a1]);

    //print_bitboard(set_blocker_combo(3, popcount(rook_masks[a1]), rook_masks[a1]));

    //std::cout << rook_shifts[a1] << std::endl;
    //print_bitboard(rook_attacks[e4][(blockers * rook_magics[e4])>>(64-rook_shifts[e4])]);

    init_bishop_attacks();

    uint64_t test = 0ULL;
    set_bit(test, c5);
    set_bit(test, g5);
    set_bit(test, h6);
    print_bitboard(test);
    //print_bitboard(bishop_masks[d7]);
    print_bitboard(generate_real_bishop_attacks(test, e3));
    print_bitboard(test);
    print_bitboard(bishop_attacks[e3][(test * bishop_magics[e3])>>(64-bishop_shifts[e3])]);
    */

    //for (int square = 0; square < 64; square++) {
        //printf("  0x%llxULL,\n", find_magics(square, bishop_shifts[square], 1));
    //}

    init_all_attacks();
    
    uint64_t test = 0ULL;
    set_bit(test, e6);
    set_bit(test, c4);
    print_bitboard(rook_attacks[e4][(test * rook_magics[e4])>>(64-rook_shifts[e4])]);

    uint64_t test2 = 0ULL;
    print_bitboard(bishop_attacks[d4][(test2 * bishop_magics[d4])>>(64-bishop_shifts[d4])]);

    return 0;
}