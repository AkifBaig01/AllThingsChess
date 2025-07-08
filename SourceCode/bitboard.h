#ifndef BITBOARD_H
#define BITBOARD_H

#include "uci.h"
#include "utils.h"

#include <string>
#include <cstdint>


// Struct to represent the full state of the game

struct full_pos {
    uint64_t bitboard[piece_no];  // 6 piece types 2 colours
    uint64_t occupancies[both_no];  // white, black and both

    // Castling rights explicitly deifned 
    bool white_king_side_castle = true;
    bool white_queen_side_castle = true;
    bool black_king_side_castle = true;
    bool black_queen_side_castle = true;
    
    int enpassant_square = -1;  // -1 if not availible otherwise square index
    int half_move_clock = 0; // keep track of the 50 move rule 
    int full_move_clock = 1; // full move numbers incremented after black to move

    int to_move = 1; // 1 - white to move, 0 - black to move

    full_pos() {
        for (int i = 0; i < piece_no; i++) {
            bitboard[i] = 0ULL;
        }
    }
};




void print_bitboard(uint64_t bitboard);

full_pos set_bitboard_pos(fen_rep& fen_pos);



#endif