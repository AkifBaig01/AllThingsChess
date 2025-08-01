#include "bitboard.h"
#include "utils.h"
#include "movegen.h"

#include <vector>
#include <cstdint>
#include <iostream>


void init_movegen() {
    init_all_attacks();
}

struct Move {
    int from_sq, to_sq;
    int promotion = empty;
};

struct MoveHistory {
    std::vector<full_pos> prev_states; 
};

MoveHistory history;


void print_moves(const Move& move) {
    std::string from = index_to_square(move.from_sq);
    std::string to = index_to_square(move.to_sq);

    std::cout << from + to << std::endl;
}


// writing some repetitive code when its tested and psuedo legal is working for all piece types
// I cna abstract away the repetitive parts of my code and wrap it in a function


std::vector<Move> gen_psuedo_moves(const full_pos& gamestate, uint64_t possible_moves, int pos) {
    std::vector<Move> moves;
    // If its white to move the knight we are concerned with must be whites 
    // Otherwise its blacks
    if (gamestate.to_move) {
        possible_moves &= ~gamestate.bitboard[white_pieces];
    }
    else {
        possible_moves &= ~gamestate.bitboard[black_pieces];
    }
    print_bitboard(possible_moves);

    // if moves availble add to move array until it ends
    while (possible_moves) {
        int current_square = get_lsb_index(possible_moves);
        possible_moves &= possible_moves - 1;

        Move current_move;
        current_move.from_sq = pos;
        current_move.to_sq = current_square;
        moves.push_back(current_move);
    }

    return moves;
}




std::vector<Move> knight_moves(const full_pos& gamestate, int pos) {
    uint64_t possible_moves = knight_attacks[pos];
    print_bitboard(possible_moves);

    return gen_psuedo_moves(gamestate, possible_moves, pos);
}

std::vector<Move> king_moves(const full_pos& gamestate, int pos) {
    uint64_t possible_moves = king_attacks[pos];
    print_bitboard(possible_moves);

    return gen_psuedo_moves(gamestate, possible_moves, pos);
}

std::vector<Move> bishop_moves(const full_pos& gamestate, int pos) {
    uint64_t mask = bishop_masks[pos];
    uint64_t occ = gamestate.bitboard[both_pieces] & mask;
    int index = ((occ * bishop_magics[pos]) >> (64-bishop_shifts[pos]));

    uint64_t possible_moves = bishop_attacks[pos][index];
    return gen_psuedo_moves(gamestate, possible_moves, pos);
}

std::vector<Move> rook_moves(const full_pos& gamestate, int pos) {
    uint64_t mask = rook_masks[pos];
    uint64_t occ = gamestate.bitboard[both_pieces] & mask;
    int index = ((occ * rook_magics[pos]) >> (64-rook_shifts[pos]));

    uint64_t possible_moves = rook_attacks[pos][index];
    return gen_psuedo_moves(gamestate, possible_moves, pos);
}


void make_move(full_pos& gamestate, Move& movestate) {
    // before we make a move, store prev state
    history.prev_states.push_back(gamestate);

    // store legal moves
    std::vector<Move> legal_moves;

    // Figure what piece are we dealing with 

}

void unmake_move() {
    
}


std::vector<Move> generate_pseudo_legal_moves(const full_pos& state);
bool is_legal(const full_pos& state, const Move& move);