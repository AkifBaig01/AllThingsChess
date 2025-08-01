#ifndef UCI_H
#define UCI_H

#include "utils.h"

#include <string>
#include <algorithm>
#include <vector>


// Represents the full fen string in a board representation
struct fen_rep {

    int squares[64];  // hold the values for board sqaures occupied and by what piece

    // Castling rights explicitly deifned 
    bool white_king_side_castle = false;
    bool white_queen_side_castle = false;
    bool black_king_side_castle = false;
    bool black_queen_side_castle = false;
    
    int enpassant_square = -1;  // -1 if not availible otherwise square index
    int half_move_clock = 0; // keep track of the 50 move rule 
    int full_move_clock = 1; // full move numbers incremented after black to move

    int to_move = 1; // 1 if white to move, 0 if black to move

    fen_rep() {
        std::fill(squares, squares + 64, empty);
    }
};


// Builds struct to represent state internally
fen_rep fen_parser(std::string fen_string);

// Prints out the full struct very useful for debugging
void helper_print(fen_rep& parsed);


#endif