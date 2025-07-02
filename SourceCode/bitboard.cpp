#include "bitboard.h"
#include "uci.h"
#include "utils.h"

#include <cstdint>
#include <iostream>
#include <stdio.h>


// Set/Get/Pop Macros can be called anymore like functions 

// Left shift by square index perform Bitwise OR with bitboard
#define set_bit(bitboard, square) (bitboard |=  (1ULL << square))

// Left shift by sqaure index perform Btiwise AND with bitbaord
#define get_bit(bitboard, square) (bitboard & (1ULL << square))

// If get bit returns true then do Bitwise XOR else return 0
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? bitboard ^= (1Ull << square) : 0)


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
};



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
            // convert file and rank into sqaure index
            int sqaure = rank * 8 + file;
            
            // print ranks 
            if (!file)
                printf("%d - ", 8 - rank);

            // Left shift 1 by the sqaure index do a bitwise and operation return 1 if ture else 0
            printf(" %d ", get_bit(bitboard, sqaure) ? 1 : 0);
        }

        // Print new line every rank 
        printf("\n");
    }

    // print board files
    printf("\n     a  b  c  d  e  f  g  h\n\n");

    // print bitbaord as an unsigned decimal number 
    printf("Bitboard: %llud\n\n", bitboard);
}

/*
=================================================
HELPER PRINT END
=================================================
*/

/*
int main(){
    // defining bitboard
    uint64_t bitboard = 0ULL;
    
    // set bits
    set_bit(bitboard, e3);
    set_bit(bitboard, e4);
    set_bit(bitboard, e5);
    set_bit(bitboard, e1);

    // show board
    print_bitboard(bitboard);

    // reset bits 
    pop_bit(bitboard, e4);

    // show board
    print_bitboard(bitboard);

    pop_bit(bitboard, e4);

    print_bitboard(bitboard);
  
}

*/

// TODO: Some tests firts, start wrting specific bitboards e.g white pawns
// Even defining the struct is useful at this stage


/*
Plan is to parse the fen string in a sperate file 
and pass a struct into here with everything ready to be made and using that struct 
update the relevant board pieces 

This will be done in the uci function a struct basically looks like the same one i used 
here in the bitboard.cpp file 
*/