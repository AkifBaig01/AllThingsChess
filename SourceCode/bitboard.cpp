#include "bitboard.h"
#include "uci.h"

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


// board sqaures
// generates constant numbers for each sqaure value e.g. e4 = 30
enum {
    a8, b8, c8, d8, e8, f8, g8, h8,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a1, b1, c1, d1, e1, f1, g1, h1
};

/*
To be used later...

"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"
"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7"
"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6"
"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5"
"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4"
"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3"
"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2"
"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
*/


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

// TODO: Some tests firts, start wrting specific bitboards e.g white pawns
// Even defining the struct is useful at this stage
