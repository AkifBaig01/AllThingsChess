#include "utils.h"
#include "bitboard.h"

#include <cstdint>

// https://github.com/paulsonkoly/chess-3/blob/main/movegen/tables.go

// Intialise global attack tables
// Pawn attacks [side][square]
uint64_t pawn_attacks[2][64];

// Knight attacks [square]
uint64_t knight_attacks[64];

// king attacks [sqaure]
uint64_t king_attacks[64];

uint64_t bishop_masks [64];
uint64_t bishop_attacks[64][512];
uint64_t rook_masks [64];
uint64_t rook_attacks[64][4096];

// Constants to be used to check for capture wrap arounds
uint64_t A_file = 0x0101010101010101;
uint64_t H_file = 0x8080808080808080;
uint64_t AB_file = A_file | (A_file << 1);
uint64_t GH_file = H_file | (H_file >> 1);




// LEAPER PIECE ATTACKS
uint64_t generate_pawn_attacks(int pawn_pos, int piece_colour) {
    // Get the bitboard with the all 0s except for the start square
    uint64_t attack_mask = 0ULL | (1ULL << pawn_pos);

    // Check if white or black pawn 
    if (piece_colour) {
        // Right shift by offset check for wrap arounds
        attack_mask = ((attack_mask >> 7) & ~A_file) | ((attack_mask >> 9) & ~H_file);
    }
    else {
        // Left shift by offset check for wrap arounds
        attack_mask = ((attack_mask << 7) & ~H_file) | ((attack_mask << 9) & ~A_file);
    }

    return attack_mask;
}


uint64_t generate_knight_attacks(int knight_pos) {
    uint64_t attack_mask = 0ULL | (1ULL << knight_pos);

    attack_mask = ((attack_mask << 17) & ~A_file) | ((attack_mask << 15) & ~H_file) |
    ((attack_mask << 10) & ~AB_file) | ((attack_mask << 6) & ~GH_file) | 
    ((attack_mask >> 17) & ~H_file) | ((attack_mask >> 15) & ~A_file) |
    ((attack_mask >> 10) & ~GH_file) | ((attack_mask >> 6) & ~AB_file);

    return attack_mask;
}

uint64_t generate_king_attacks(int king_pos) {
    // king offsets 8,9,7,1
    // should be pretty easy from there just try left and right shifts
    // and test out the h a and ocmbination files for where its needed

    uint64_t attack_mask = 0ULL | (1ULL << king_pos);

    attack_mask = (attack_mask >> 8) | 
    ((attack_mask >> 9) & ~H_file) |
    ((attack_mask >> 7) & ~A_file) |
    ((attack_mask >> 1) & ~H_file) |
    (attack_mask << 8) | 
    ((attack_mask << 9) & ~A_file) |
    ((attack_mask << 7) & ~H_file) |
    ((attack_mask << 1) & ~A_file);;

    return attack_mask;
}

// LEAPER PIECE ATTACKS


// SLIDING PIECE ATTACKS

uint64_t generate_bishop_masks(int bishop_pos){
    uint64_t relevant_occupancy_squares = 0ULL;

    // init rank and files
    int rank, file;

    // init the rank and files we are interested in 
    int target_rank = bishop_pos / 8;
    int target_file = bishop_pos % 8;

    // mask relevant occupancy squares
    // increments rank and files along the diagonal ignoring edge squares
    // run four loops for the four directions 
    // using standard formula to convert rank and file to square index
    for (rank = target_rank + 1, file = target_file + 1; rank <= 6 && file <= 6; rank++, file++) {
        relevant_occupancy_squares |= (1ULL << (rank * 8 + file));
    }


    for (rank = target_rank - 1, file = target_file + 1; rank >= 1 && file <= 6; rank--, file++) {
        relevant_occupancy_squares |= (1ULL << (rank * 8 + file));
    }


    for (rank = target_rank + 1, file = target_file - 1; rank <= 6 && file >= 1; rank++, file--) {
        relevant_occupancy_squares |= (1ULL << (rank * 8 + file));
    }


    for (rank = target_rank - 1, file = target_file - 1; rank >= 1 && file >= 1; rank--, file--) {
        relevant_occupancy_squares |= (1ULL << (rank * 8 + file));
    }

    return relevant_occupancy_squares;
}

uint64_t generate_rook_masks(int rook_pos){
    uint64_t relevant_occupancy_squares = 0ULL;

    // init rank and files
    int rank, file;

    // init the rank and files we are interested in 
    int target_rank = rook_pos / 8;
    int target_file = rook_pos % 8;

    // mask relevant occupancy squares
    // increments rank and files along the diagonal ignoring edge squares
    // run four loops for the four directions 
    // using standard formula to convert rank and file to square index
    for (rank = target_rank + 1; rank <= 6; rank++) {
        relevant_occupancy_squares |= (1ULL << (rank * 8 + target_file));
    }


    for (rank = target_rank - 1; rank >= 1; rank--) {
        relevant_occupancy_squares |= (1ULL << (rank * 8 + target_file));
    }

    
    for (file = target_file + 1; file <= 6; file++) {
        relevant_occupancy_squares |= (1ULL << (target_rank * 8 + file));
    }


    for (file = target_file - 1; file >= 1; file--) {
        relevant_occupancy_squares |= (1ULL << (target_rank * 8 + file));
    }
    
    return relevant_occupancy_squares;
    
}

// ATTACK TABLES

// GENERATE ALL ATTACK TABLES

void all_attack_tables(){

    // Pawn attacks, loop for both black and white pieces
    for (int piece_colour = 0; piece_colour < 2; piece_colour++) {
        for (int squares = 0; squares < 64; squares++){
            pawn_attacks[piece_colour][squares] = generate_pawn_attacks(squares, piece_colour);
        }
    }

    // Knight attacks and King attacks
    for (int squares = 0; squares < 64; squares++) {
        knight_attacks[squares] = generate_knight_attacks(squares);
        king_attacks[squares] = generate_king_attacks(squares);
        bishop_masks[squares] = generate_bishop_masks(squares);
        rook_masks[squares] = generate_rook_masks(squares);
    }
    
}

/*
For these magic tbales the process seems to be the same I will use rooks as an example
do rook_masks so this says if a rook is lets a1 it can attack all the sqaures
horizontal and vertically. then to bitwise and with all the occupency and gives you the
blocker combo. turn that blocker config into a unique index for your table using the magic numbers
look that up on rook_attack table. then filter out with your same colour pices maybe with a
bitwise xor and that gives you the final attacks.
*/



// Legality checking and a lot of other stuff 