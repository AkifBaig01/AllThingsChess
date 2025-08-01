#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <cstdint>

// -------- ATTACK TABLES ----------
extern uint64_t pawn_attacks[2][64];
extern uint64_t knight_attacks[64];
extern uint64_t king_attacks[64];

extern uint64_t bishop_attacks[64][512];
extern uint64_t rook_attacks[64][4096];
extern uint64_t rook_masks[64];
extern uint64_t bishop_masks[64];
// -------- ATTACK TABLES -----------

// -------- MAGICS & SHIFTS ---------
extern uint64_t bishop_magics[64];
extern uint64_t rook_magics[64];
extern int bishop_shifts[64];
extern int rook_shifts[64];
// -------- MAGICS & SHIFTS ---------


// ------- INTIALISER --------
void init_all_attacks();
// ------- INTIALISER --------

#endif