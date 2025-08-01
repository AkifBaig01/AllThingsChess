#include "movegen.h"
#include "utils.h"
#include "bitboard.h"


#include <cstdint>
#include <string.h>
#include <assert.h>
#include <random>
#include <iostream>


// Intialise global attack tables
// Pawn attacks [side][square]
uint64_t pawn_attacks[2][64];

// Knight attacks [square]
uint64_t knight_attacks[64];

// king attacks [sqaure]
uint64_t king_attacks[64];

// piece_mask [square]
// piece_attack[square][index]
uint64_t bishop_masks[64];
uint64_t bishop_attacks[64][512];
uint64_t rook_masks [64];
uint64_t rook_attacks[64][4096];

// piece magics & shifts 
uint64_t bishop_magics[64] = {
    0x8040100401104118ULL,
    0x2002044404204084ULL,
    0x110008091000000ULL,
    0x302092004000084aULL,
    0x604042004c00800ULL,
    0x2012086208000000ULL,
    0x8140980402a00002ULL,
    0x1030805011880ULL,
    0x14004a044a10200ULL,
    0x4000240802024a10ULL,
    0x4088040112020129ULL,
    0x8b04082043400100ULL,
    0x4208040308002089ULL,
    0x3841092818405000ULL,
    0x140004202104202ULL,
    0x7088008044100401ULL,
    0x81010200800c0ULL,
    0x5012001220210ULL,
    0x4081800302200ULL,
    0x204800089200c002ULL,
    0x9a02221400a00006ULL,
    0x60090002004202c0ULL,
    0x2020402088080880ULL,
    0x2002038420842400ULL,
    0x890406010040100ULL,
    0x10040008015400ULL,
    0x4010884010004e00ULL,
    0x40040000410120ULL,
    0x2021001001004004ULL,
    0x50024001825000ULL,
    0x8020011821588ULL,
    0x3902001040240ULL,
    0x414104188890200ULL,
    0x2056081100e00ULL,
    0x8090840104100043ULL,
    0x40a0080208e0200ULL,
    0x4120464040040100ULL,
    0x1001210100220040ULL,
    0x2082210004220ULL,
    0x4008084880004200ULL,
    0x1029410c140a810ULL,
    0x20822028c0800ULL,
    0x10608405801000ULL,
    0x400064010480200ULL,
    0x608042200a010900ULL,
    0x8081804a0c810ULL,
    0x8110142040404ULL,
    0x240800a082040891ULL,
    0x2442020160080900ULL,
    0x8240a602104c0040ULL,
    0x2009060520880000ULL,
    0x1028440084040008ULL,
    0xc0405040000ULL,
    0x81001020520ULL,
    0x10602254004080ULL,
    0x5080084008004ULL,
    0x4802008200900408ULL,
    0x30062482582000ULL,
    0x810400020d00a810ULL,
    0x9405202020840400ULL,
    0x804000020024c10ULL,
    0x4002160a0a42120ULL,
    0x180842100a320441ULL,
    0x23020208060582ULL,
};
int bishop_shifts [64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 5, 5, 5, 5, 5, 5, 6,
};
uint64_t rook_magics[64] = {
    0x408002805025c000ULL,
    0x40100040002000ULL,
    0x20010a202884080ULL,
    0x200040820104200ULL,
    0x1200080410200200ULL,
    0x450004000900082aULL,
    0x80010002000080ULL,
    0x8200004210850024ULL,
    0x2300802080004000ULL,
    0x5400250002006ULL,
    0x1000801000802000ULL,
    0x5001001002508ULL,
    0x45001100040802ULL,
    0x1012003492000810ULL,
    0x2a02004448010200ULL,
    0x2000d00441082ULL,
    0x1828001400020ULL,
    0x49000402000400cULL,
    0x21010010200042ULL,
    0x20a0020120040ULL,
    0x180808004000800ULL,
    0x2790808004000200ULL,
    0x840068010210ULL,
    0xa4120010410084ULL,
    0x48802980084000ULL,
    0x10005040002001ULL,
    0x410100200010ULL,
    0x4010041180080080ULL,
    0x8400080080800400ULL,
    0x4040080800200ULL,
    0x880010400021008ULL,
    0xc110820000c32cULL,
    0x24401020800482ULL,
    0x9004812004804005ULL,
    0x200080801000ULL,
    0x800100021000900ULL,
    0x4800400800802ULL,
    0x1920020080800400ULL,
    0x80204008110ULL,
    0x1006082000401ULL,
    0x46400a20808000ULL,
    0x440058020048040ULL,
    0xd018200010008080ULL,
    0x18002100d0050008ULL,
    0x1008000411010008ULL,
    0x42002010040400ULL,
    0x100114210040088ULL,
    0x200006a400420013ULL,
    0x402010080205a00ULL,
    0x802000401480ULL,
    0x1087002001104100ULL,
    0x6021208402200ULL,
    0x2808080080040080ULL,
    0x1000020004008080ULL,
    0x10010802100400ULL,
    0x10a8008401004200ULL,
    0x80492080010011c1ULL,
    0x850102100804005ULL,
    0x102021040082082ULL,
    0x140900100600825ULL,
    0x80a001410082132ULL,
    0x451000802140003ULL,
    0x804100088010204ULL,
    0x180800810030440aULL,
};
int rook_shifts[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	12, 11, 11, 11, 11, 11, 11, 12,
};

// Constants to be used to check for capture wrap arounds
uint64_t A_file = 0x0101010101010101;
uint64_t H_file = 0x8080808080808080;
uint64_t AB_file = A_file | (A_file << 1);
uint64_t GH_file = H_file | (H_file >> 1);

// ALL ATTACK TABLES

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

uint64_t generate_real_bishop_attacks(uint64_t blockers, int bishop_pos) {
    uint64_t attacks = 0ULL;

    // init rank and files
    int rank, file;

    // init the rank and files we are interested in 
    int target_rank = bishop_pos / 8;
    int target_file = bishop_pos % 8;

    // Essentially same as mask but goes to edge and drops at first blocker on file rank
    // North-East
    for (rank = target_rank + 1, file = target_file + 1; rank <= 7 && file <= 7; rank++, file++) {
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << rank * 8 + file)) break;
    }

    // South-East
    for (rank = target_rank - 1, file = target_file + 1; rank >= 0 && file <= 7; rank--, file++) {
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << rank * 8 + file)) break;
    }

    // North-West
    for (rank = target_rank + 1, file = target_file - 1; rank <= 7 && file >= 0; rank++, file--) {
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << rank * 8 + file)) break;
    }

    // South-West
    for (rank = target_rank - 1, file = target_file - 1; rank >= 0 && file >= 0; rank--, file--) {
        attacks |= (1ULL << (rank * 8 + file));
        if (blockers & (1ULL << rank * 8 + file)) break;
    }

    return attacks;
}


uint64_t generate_rook_masks(int rook_pos){
    uint64_t relevant_occupancy_squares = 0ULL;

    // init rank and files
    int rank, file;

    // init the rank and files we are interested in 
    int target_rank = rook_pos / 8;
    int target_file = rook_pos % 8;


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

uint64_t generate_real_rook_attacks(uint64_t blockers, int rook_pos) {
    uint64_t attacks = 0ULL;

    // init rank and files
    int rank, file;

    // init the rank and files we are interested in 
    int target_rank = rook_pos / 8;
    int target_file = rook_pos % 8;

    // East
    for (rank = target_rank + 1; rank <= 7; rank++) {
        attacks |= (1ULL << (rank * 8 + target_file));
        if (blockers & (1ULL << rank * 8 + target_file)) break;
    }

    // West
    for (rank = target_rank - 1; rank >= 0; rank--) {
        attacks |= (1ULL << (rank * 8 + target_file));
        if (blockers & (1ULL << rank * 8 + target_file)) break;
    }

    // North
    for (file = target_file + 1; file <= 7; file++) {
        attacks |= (1ULL << (target_rank * 8 + file));
        if (blockers & (1ULL << target_rank * 8 + file)) break;
    }

    // South
    for (file = target_file - 1; file >= 0; file--) {
        attacks |= (1ULL << (target_rank * 8 + file));
        if (blockers & (1ULL << target_rank * 8 + file)) break;
    }
    
    return attacks;
}

// Function will generate all possible occupancy variations
// Using bit tricks e.g. all blocker combination are numbered 0 - 2^n -1
uint64_t set_blocker_combo(int blocker_number, int bits_in_mask, uint64_t attack_mask) {
    uint64_t blocker_combo = 0ULL;

    for (int possible_sqaure_no = 0; possible_sqaure_no < bits_in_mask; possible_sqaure_no++) {
        int current_square = get_lsb_index(attack_mask);
        // gets rid of lsb
        attack_mask &= attack_mask - 1;

        if (blocker_number & (1ULL << possible_sqaure_no)) {
            blocker_combo |= 1ULL << current_square;
        }
    }
    
    return blocker_combo;
}

// SLIDING PIECE ATTACKS

// ATTACK TABLES


// Change this later to call two init functions and thats it
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

// Generate rook attack tables
void init_rook_attacks() {
    memset(rook_attacks, 0xFF, sizeof(rook_attacks));
    for (int square = 0; square < 64; square++) {
        uint64_t rook_mask = rook_masks[square];
        int current_bits = popcount(rook_mask);
        int index =  1 << rook_shifts[square];

        for (int j = 0; j < index; j++) {
            uint64_t current_blockers = set_blocker_combo(j, current_bits, rook_mask);
            uint64_t store = (current_blockers * rook_magics[square]) >> (64 - rook_shifts[square]);
            assert(rook_attacks[square][store] == 0xFFFFFFFFFFFFFFFFULL && "Magic‐table collision on rook square");            
            rook_attacks[square][store] = generate_real_rook_attacks(current_blockers, square);
        }
    }
}

void init_bishop_attacks() {
    memset(bishop_attacks, 0xFF, sizeof(bishop_attacks));
    for (int square = 0; square < 64; square++) {
        uint64_t bishop_mask = bishop_masks[square];
        int current_bits = popcount(bishop_mask);
        int index =  1 << bishop_shifts[square];

        for (int j = 0; j < index; j++) {
            uint64_t current_blockers = set_blocker_combo(j, current_bits, bishop_mask);
            uint64_t store = (current_blockers * bishop_magics[square]) >> (64 - bishop_shifts[square]);
            assert(bishop_attacks[square][store] == 0xFFFFFFFFFFFFFFFFULL && "Magic‐table collision on bishop square");
            bishop_attacks[square][store] = generate_real_bishop_attacks(current_blockers, square);
        }
    }
}    

std::mt19937_64 gen(17020);

uint64_t random_number() {
    return gen() & gen() & gen();
}

uint64_t find_magics(int square, int bit_shift, int bishop) {
    uint64_t mask, blocker_combo[4096], real_attacks[4096], used[4096], magic, index;
    int i, k, bits_in_mask, fail;

    mask = bishop? generate_bishop_masks(square) : generate_rook_masks(square);
    bits_in_mask = popcount(mask);

    for (i = 0; i < (1 << bits_in_mask); i++) {
        blocker_combo[i] = set_blocker_combo(i, bits_in_mask, mask);
        real_attacks[i] = bishop? generate_real_bishop_attacks(blocker_combo[i], square) : generate_real_rook_attacks(blocker_combo[i], square);
    }

    for (k = 0; k < 100000000; k++) {
        magic = random_number();
        if(popcount((mask * magic) & 0xFF00000000000000ULL) < 6) continue;
        for(i = 0; i < 4096; i++) used[i] = 0ULL;
        for(i = 0, fail = 0; !fail && i < (1 << bits_in_mask); i++) {
            index = (blocker_combo[i] * magic) >> (64 - bit_shift);
            if(used[index] == 0ULL) used[index] = real_attacks[i];
            else if(used[index] != real_attacks[i]) fail = 1;    
        }
        if(!fail) return magic; 
    }
    printf("***Failed***\n");
    return 0ULL;
}

void init_all_attacks() {
    all_attack_tables();
    init_rook_attacks();
    init_bishop_attacks();
}