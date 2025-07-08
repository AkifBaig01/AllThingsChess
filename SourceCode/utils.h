#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <bitset>
#include <cctype>
#include <algorithm>
#include <vector>

// ENUM DECLARTIONS

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

// indexes to represent full position for each colour
enum {
    white_pieces, black_pieces, both_pieces,
    both_no
};

// For piece case tables and indexing for array, provides constant values 
enum {
    white_pawns, white_knights, white_bishops, white_rooks, white_queens, white_king,
    black_pawns, black_knights, black_bishops, black_rooks, black_queens, black_king,
    piece_no, empty = -1
};

// Great for side to move indexing
enum {
    white = 1, black = 0
};


// ENUM DECLARATIONS

// FUNCTION DECLARATIONS
std::vector<std::string> split(const std::string& lines, char delim = ' ');

int sqaure_to_index(const std::string& square);

// FUNCTION DECLARATIONS






#endif