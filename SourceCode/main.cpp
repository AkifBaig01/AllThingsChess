#include "uci.h"
#include "bitboard.h"
#include "utils.h"
#include "movegen.h"

// For testing update with header file 
#include "moves.cpp"

#include <iostream>
#include <string>


int main() {
    
    std::string fen = "6k1/8/2P1P3/3B4/2p5/8/7P/6K1 w - - 0 1";

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
    
    std::cout << "This is the end of this section \n\n\n\n";

    uint64_t test = 0ULL;

    set_bit(test, c6);
    set_bit(test, e6);
    set_bit(test, c4);


    init_all_attacks();
    
    //print_bitboard(bishop_attacks[d5][(test * bishop_magics[d5]) >> bishop_shifts[d5]]);

    std::vector<Move> psuedo_bishop = bishop_moves(represent, d5);

    for (Move move : psuedo_bishop) {
        print_moves(move);
    }

    return 0;
}