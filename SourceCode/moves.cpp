#include "bitboard.h"
#include "utils.h"
#include "movegen.h"

#include <vector>
#include <cstdint>
#include <iostream>

// Please create a named enum for white pieces instead of just hard coding 1s everywhere
// Very bad practice :(
// for all these things you really need to think about how to best implement the 
// white an dblack instead of always having the if else conditions




void init_movegen() {
    init_all_attacks();
}

struct Move {
    int from_sq, to_sq;
    int promotion = 0;
    bool ep = false;
    bool double_push = false;
    bool castle = false;
};

struct Undo {
    Move move;
    int captured_piece; // -1 if none
    int captured_square;
    int mover_piece;
    bool Wkingside;
    bool WQueenside;
    bool BKingside;
    bool BQueenside;
    int old_ep_square;
    int old_half_move;
    
};

struct MoveHistory {
    std::vector<full_pos> prev_states; 
};

std::vector<Undo> history;


void print_moves(const Move& move) {
    std::string from = index_to_square(move.from_sq);
    std::string to = index_to_square(move.to_sq);

    std::cout << from + to << std::endl;
}

// Global Constants

// white pawn ranks
uint64_t rank_3 = 280375465082880ULL;
uint64_t rank_8 = 255ULL;

// black pawn ranks 
uint64_t rank_6 = 16711680ULL;
uint64_t rank_1 = 18374686479671623680ULL;
// rank rank 6 and rank 1








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


bool is_sq_attacked(const full_pos& gamestate, int king_pos) {
    bool white = gamestate.to_move;

    // BISHOP ATTACKS
    uint64_t Bmask = bishop_masks[king_pos];
    uint64_t Bocc = gamestate.bitboard[both_pieces] & Bmask;
    int Bindex = ((Bocc * bishop_magics[king_pos]) >> (64-bishop_shifts[king_pos]));
    uint64_t Battacks = bishop_attacks[king_pos][Bindex];

    // ROOK ATTACKS
    uint64_t Rmask = rook_masks[king_pos];
    uint64_t Rocc = gamestate.bitboard[both_pieces] & Rmask;
    int Rindex = ((Rocc * rook_magics[king_pos]) >> (64-rook_shifts[king_pos]));
    uint64_t Rattacks = rook_attacks[king_pos][Rindex];

    if (white) {

        if (pawn_attacks[1][king_pos] & gamestate.bitboard[black_pawns]) return true;
        if (knight_attacks[king_pos] & gamestate.bitboard[black_knights]) return true;
        if (king_attacks[king_pos] & gamestate.bitboard[black_king]) return true;
        if (Battacks & (gamestate.bitboard[black_bishops] | gamestate.bitboard[black_queens])) return true;
        if (Rattacks & (gamestate.bitboard[black_rooks] | gamestate.bitboard[black_queens])) return true;
    
    } else {

        if (pawn_attacks[0][king_pos] & gamestate.bitboard[white_pawns]) return true;
        if (knight_attacks[king_pos] & gamestate.bitboard[white_knights]) return true;
        if (king_attacks[king_pos] & gamestate.bitboard[white_king]) return true;
        if (Battacks & (gamestate.bitboard[white_bishops] | gamestate.bitboard[white_queens])) return true;
        if (Rattacks & (gamestate.bitboard[white_rooks] | gamestate.bitboard[white_queens])) return true;
    
    }

    return false;
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

std::vector<Move> queen_moves(const full_pos& gamestate, int pos) {
    // Bishop Moves
    uint64_t maskB = bishop_masks[pos];
    uint64_t occB = gamestate.bitboard[both_pieces] & maskB;
    int indexB = ((occB * bishop_magics[pos]) >> (64-bishop_shifts[pos]));
    uint64_t possible_bishop_moves = bishop_attacks[pos][indexB];

    // Rook Moves
    uint64_t maskR = rook_masks[pos];
    uint64_t occR = gamestate.bitboard[both_pieces] & maskR;
    int indexR = ((occR * rook_magics[pos]) >> (64-rook_shifts[pos]));
    uint64_t possible_rook_moves = rook_attacks[pos][indexR];

    uint64_t possible_queen_moves = possible_bishop_moves | possible_rook_moves;
    return gen_psuedo_moves(gamestate, possible_queen_moves, pos);
}



// Could refactor these into helper functions nothing too crazy just 
// Popping bits and getting their index and abstracting away make move logic with
// Default parameters, forgot they existed to be honest

std::vector<Move> white_pawn_moves(const full_pos& gamestate) {
    std::vector<Move> moves;

    // Intialise pawns, free squares and black pieces
    uint64_t whitepawns = gamestate.bitboard[white_pawns];
    uint64_t clear_sq = ~gamestate.bitboard[both_pieces];
    uint64_t blackpieces = gamestate.bitboard[black_pieces];

    // Generate bitboards for all possible moves besides captures and enpassants
    uint64_t single_push = (whitepawns >> 8) & clear_sq;
    uint64_t quiet_single_push = single_push & ~rank_8;
    uint64_t double_push = ((single_push & rank_3) >> 8) & clear_sq;
    uint64_t promotions = single_push & rank_8;

// ------- handle push moves and promos from pushes ----------
    while (quiet_single_push) {

        Move move;
        move.to_sq = get_lsb_index(quiet_single_push);
        move.from_sq = move.to_sq + 8;
        moves.push_back(move);

        quiet_single_push &= quiet_single_push - 1;
    }

    while (double_push) {

        Move move;
        move.to_sq = get_lsb_index(double_push);
        move.from_sq = move.to_sq + 16;
        move.double_push = true;
        moves.push_back(move);

        double_push &= double_push - 1;
    }

    while (promotions) {

        Move move;
        move.to_sq = get_lsb_index(promotions);
        move.from_sq = move.to_sq + 8;
        for (auto promo_piece : {white_queens, white_knights}) {
            move.promotion = promo_piece;
            moves.push_back(move);
        }

        promotions &= promotions - 1;

    }

// ------- Handle captures only ---------
    while (whitepawns) {
        int from = get_lsb_index(whitepawns);
        uint64_t attacks = pawn_attacks[1][from] & blackpieces;

        while (attacks) {
            int to = get_lsb_index(attacks);
            Move move;
            move.from_sq = from;
            move.to_sq = to;

            // check for promotion 
            if ((1ULL << to) & rank_8) {
                for (auto promo_piece : {white_queens, white_knights}) {
                    move.promotion = promo_piece;
                    moves.push_back(move);
                }
            } else {
                moves.push_back(move);
            }

            attacks &= attacks - 1;
        }
        
        whitepawns &= whitepawns - 1;
    }

// ------- handle enpassants --------
    whitepawns = gamestate.bitboard[white_pawns];
    int ep = gamestate.enpassant_square;
    if (ep != empty) {
        uint64_t ep_bb = (1ULL << ep);

        while (whitepawns) {
            int from = get_lsb_index(whitepawns);
            if (pawn_attacks[1][from] & ep_bb) {

                Move move;
                move.from_sq = from;
                move.to_sq = ep;
                move.ep = true;

                moves.push_back(move);
            }
            whitepawns &= whitepawns - 1;
        }
    }

    return moves;
}



std::vector<Move> black_pawn_moves(const full_pos& gamestate) {
    std::vector<Move> moves;

    // Intialise pawns, free squares and black pieces
    uint64_t blackpawns = gamestate.bitboard[black_pawns];
    uint64_t clear_sq = ~gamestate.bitboard[both_pieces];
    uint64_t whitepieces = gamestate.bitboard[white_pieces];

    // Generate bitboards for all possible moves besides captures and enpassants
    uint64_t single_push = (blackpawns << 8) & clear_sq;
    uint64_t quiet_single_push = single_push & ~rank_1;
    uint64_t double_push = ((single_push & rank_6) << 8) & clear_sq;
    uint64_t promotions = single_push & rank_1;

// ------- handle push moves and promos from pushes ----------
    while (quiet_single_push) {

        Move move;
        move.to_sq = get_lsb_index(quiet_single_push);
        move.from_sq = move.to_sq - 8;
        moves.push_back(move);

        quiet_single_push &= quiet_single_push - 1;
    }

    while (double_push) {

        Move move;
        move.to_sq = get_lsb_index(double_push);
        move.from_sq = move.to_sq - 16;
        move.double_push = true;
        moves.push_back(move);

        double_push &= double_push - 1;
    }

    while (promotions) {

        Move move;
        move.to_sq = get_lsb_index(promotions);
        move.from_sq = move.to_sq - 8;
        for (auto promo_piece : {black_queens, black_knights}) {
            move.promotion = promo_piece;
            moves.push_back(move);
        }

        promotions &= promotions - 1;

    }

// ------- Handle captures only ---------
    while (blackpawns) {
        int from = get_lsb_index(blackpawns);
        uint64_t attacks = pawn_attacks[0][from] & whitepieces;

        while (attacks) {
            int to = get_lsb_index(attacks);
            Move move;
            move.from_sq = from;
            move.to_sq = to;

            // check for promotion 
            if ((1ULL << to) & rank_1) {
                for (auto promo_piece : {black_queens, black_knights}) {
                    move.promotion = promo_piece;
                    moves.push_back(move);
                }
            } else {
                moves.push_back(move);
            }

            attacks &= attacks - 1;
        }
        
        blackpawns &= blackpawns - 1;
    }

// ------- handle enpassants --------
    blackpawns = gamestate.bitboard[black_pawns];
    int ep = gamestate.enpassant_square;
    if (ep != empty) {
        uint64_t ep_bb = (1ULL << ep);

        while (blackpawns) {
            int from = get_lsb_index(blackpawns);
            if (pawn_attacks[0][from] & ep_bb) {

                Move move;
                move.from_sq = from;
                move.to_sq = ep;
                move.ep = true;

                moves.push_back(move);
            }
            blackpawns &= blackpawns - 1;
        }
    }

    return moves;
}



std::vector<Move> castling(const full_pos& gamestate) {
    std::vector<Move> moves;
    uint64_t occupancy = gamestate.bitboard[both_pieces];

    // Pre-computed these just to aid redability
    bool Wkingside_clear = !(occupancy & ((1ULL << f1) | (1ULL << g1)));
    bool Wqueenside_clear = !(occupancy & ((1ULL << b1) | (1ULL << c1) | (1ULL << d1)));
    bool Bkingside_clear = !(occupancy & ((1ULL << f8) | (1ULL << g8)));
    bool Bqueenside_clear = !(occupancy & ((1ULL << b8) | (1ULL << c8) | (1ULL << d8)));

    // White Castling
    if (gamestate.white_king_side_castle && Wkingside_clear) {

        Move move;
        move.from_sq = e1;
        move.to_sq = g1;
        move.castle = true;
        moves.push_back(move);

    }

    if (gamestate.white_queen_side_castle && Wqueenside_clear) {

        Move move;
        move.from_sq = e1;
        move.to_sq = c1;
        move.castle = true;
        moves.push_back(move);

    }

    // Black Castling
    if (gamestate.black_king_side_castle && Bkingside_clear) {

        Move move;
        move.from_sq = e8;
        move.to_sq = g8;
        move.castle = true;
        moves.push_back(move);

    }

    if (gamestate.black_queen_side_castle && Bqueenside_clear) {

        Move move;
        move.from_sq = e8;
        move.to_sq = c8;
        move.castle = true;
        moves.push_back(move);

    }

    return moves;
}




// Once confirmed correct refactor all of this into a more cleaner code 
template <typename Func>
void append_moves_from_bitboard(std::vector<Move>& all_moves, const full_pos& gamestate, uint64_t bitboard, Func move_generator) {
    while (bitboard) {
        int pos = get_lsb_index(bitboard);
        bitboard &= bitboard - 1; // clear LSB

        auto moves = move_generator(gamestate, pos);
        all_moves.insert(all_moves.end(), moves.begin(), moves.end());
    }
}



std::vector<Move> psuedo_moves(const full_pos& gamestate) {
    std::vector<Move> all_moves;

    if (gamestate.to_move) {

        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[white_knights], knight_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[white_bishops], bishop_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[white_rooks], rook_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[white_queens], queen_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[white_king], king_moves);
        auto Wpawns = white_pawn_moves(gamestate);
        all_moves.insert(all_moves.end(), Wpawns.begin(), Wpawns.end());

    } else {

        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[black_knights], knight_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[black_bishops], bishop_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[black_rooks], rook_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[black_queens], queen_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[black_king], king_moves);
        auto Bpawns = black_pawn_moves(gamestate);
        all_moves.insert(all_moves.end(), Bpawns.begin(), Bpawns.end());

    }

    auto castles = castling(gamestate);
    all_moves.insert(all_moves.end(), castles.begin(), castles.end());

    return all_moves;
}






}

void unmake_move() {
    
    for (Move move : psuedo_legal) {
        // Make move will flip side to move which will be opposite for side to move
        // in the sq attacked func either modify or pass parameter with !
        make_move();
        if (is_sq_attacked) {
            // discard
        } else {
            // 
        }
    }
}
*/