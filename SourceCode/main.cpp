#include "uci.h"
#include "bitboard.h"
#include "utils.h"
#include "movegen.h"

// For testing update with header file 
#include "moves.cpp"

#include <iostream>
#include <string>


int main() {
    
    std::string fen = "8/8/8/8/1b6/8/8/4K3 w - - 0 1";

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

    init_all_attacks();
    
    //print_bitboard(bishop_attacks[d5][(test * bishop_magics[d5]) >> bishop_shifts[d5]]);

    std::vector<Move> psuedo_white_pawn = psuedo_moves(represent);
    int total_moves = 0;

    for (Move move : psuedo_white_pawn) {
        print_moves(move);
        total_moves++;
    }
    
    std::cout << total_moves << std::endl;

    uint64_t test = 0ULL;

    set_bit(test, a1);
    set_bit(test, b1);
    set_bit(test, c1);
    set_bit(test, d1);
    set_bit(test, e1);
    set_bit(test, f1);
    set_bit(test, g1);
    set_bit(test, h1);

    if (is_sq_attacked(represent, e1)) std::cout << "1 I am attacked" << std::endl;
    else std::cout << "0 Not attacked" << std::endl;
    return 0;
}