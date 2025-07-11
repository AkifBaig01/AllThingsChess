#include "bitboard.h"
#include "uci.h"
#include "utils.h"

#include <cstdint>
#include <iostream>
#include <stdio.h>


// Set/Get/Pop Macros can be called anymore like functions 

// Set these to inline functions later
// Left shift by square index perform Bitwise OR with bitboard
#define set_bit(bitboard, square) (bitboard |=  (1ULL << square))

// Left shift by square index perform Btiwise AND with bitbaord
#define get_bit(bitboard, square) (bitboard & (1ULL << square))

// If get bit returns true then do Bitwise XOR else return 0
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? bitboard ^= (1ULL << square) : 0)




/*
================================================
HELPER PRINT FOR DEBUGGING
================================================
*/

void print_bitboard(uint64_t bitboard) {
    // Loop over ranks
    for (int rank = 0; rank <8; rank++) {
        // Loop over board files
        for (int file = 0; file < 8; file++) {
            // convert file and rank into square index
            int square= rank * 8 + file;
            
            // print ranks 
            if (!file)
                printf("%d - ", 8 - rank);

            // Left shift 1 by the square index do a bitwise and operation return 1 if ture else 0
            printf(" %d ", get_bit(bitboard, square) ? 1 : 0);
        }

        // Print new line every rank 
        printf("\n");
    }

    // print board files
    printf("\n     a  b  c  d  e  f  g  h\n\n");

    // print bitbaord as an unsigned decimal number 
    printf("Bitboard: %llud\n\n", bitboard);
}




full_pos set_bitboard_pos(fen_rep& fen_pos) {
    // Intialise struct to build full postition
    full_pos full_position;

    // Build bitboards
    int i = 0;
    for (int piece : fen_pos.squares) {
        if (piece != empty) {
            set_bit(full_position.bitboard[piece], i);
        }
    i += 1;
    }

    // White Castling
    full_position.white_king_side_castle = fen_pos.white_king_side_castle;
    full_position.white_queen_side_castle = fen_pos.white_queen_side_castle;

    // Black Castling
    full_position.black_king_side_castle = fen_pos.black_king_side_castle;
    full_position.black_queen_side_castle = fen_pos.black_queen_side_castle;

    // Setting the rest of the flags
    full_position.enpassant_square = fen_pos.enpassant_square;

    full_position.half_move_clock = fen_pos.half_move_clock;

    full_position.full_move_clock = fen_pos.full_move_clock;

    full_position.to_move = fen_pos.to_move;

    full_position.bitboard[white_pieces] = full_position.bitboard[white_pawns] | full_position.bitboard[white_knights] | full_position.bitboard[white_bishops] | full_position.bitboard[white_rooks] | full_position.bitboard[white_queens] | full_position.bitboard[white_king];
    full_position.bitboard[black_pieces] = full_position.bitboard[black_pawns] | full_position.bitboard[black_knights] | full_position.bitboard[black_bishops] | full_position.bitboard[black_rooks] | full_position.bitboard[black_queens] | full_position.bitboard[black_king];

    full_position.bitboard[both_pieces] = full_position.bitboard[white_pieces] | full_position.bitboard[black_pieces];
    
    return full_position;
}
