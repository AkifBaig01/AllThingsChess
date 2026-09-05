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

struct MoveList {
    Move moves[256];
    int count = 0;
};

// Function to generate all legal moves
MoveList legal_move_gen(full_pos &state);

// Function to generate psuedo legal moves
MoveList psuedo_moves(const full_pos& gamestate);

// Apply a move onto the board and accurately update state
void make_move(full_pos& state, Move& move);

// Undo the move always come as a pair never use unmake without its corresponding make 
void unmake_move(full_pos& state);

// Useful function
bool is_sq_attacked(const full_pos& gamestate, int king_pos, int side_to_move);

// display the move in algabraiec notation without flags
void print_moves(const Move& move);


#endif