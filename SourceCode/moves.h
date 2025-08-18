#ifndef MOVES_H
#define MOVES_H

#include "bitboard.h"

#include <vector>

// How each move will be represented
struct Move {
    int from_sq, to_sq;
    int promotion = 0;
    bool ep = false;
    bool double_push = false;
    bool castle = false;
};

// Function to generate all legal moves
//std::vector<Move> legal_move_gen(full_pos &state);


#endif