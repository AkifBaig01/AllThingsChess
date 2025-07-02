#ifndef UCI_H
#define UCI_H

#include <cstdint>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <bitset>
#include <cctype>
#include <algorithm>
#include <vector>


// Struct to represetn board state from a given fen string
struct fen_rep {

    int squares[64];

    bool white_king_side_castle;
    bool white_queen_side_castle;
    bool black_king_side_castle;
    bool black_queen_side_castle;
    
    int enpassant_square;
    int half_move_clock; 
    int full_move_clock;

    int to_move;

    fen_rep();
};

// Builds struct to represent state internally
fen_rep fen_parser(std::string fen_string);

// Prints out the full struct very useful for debugging
void helper_print(fen_rep& parsed);


#endif