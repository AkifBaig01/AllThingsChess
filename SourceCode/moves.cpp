#include "moves.h"
#include "bitboard.h"
#include "utils.h"
#include "movegen.h"

#include <vector>
#include <cstdint>
#include <iostream>

void init_movegen() {
    init_all_attacks();
}

struct Undo {
    Move move;
    int captured_piece; // -1 if none
    int captured_square;
    int mover_piece;

    bool Wkingside;
    bool WQueenside;  // Can change these to uint8_t and use bit masks
    bool BKingside;
    bool BQueenside;

    int old_ep_square;
    int old_half_move;
    int old_full_move;
    
};
struct HistoryList{
    Undo history[256];
    int count = 0;
};

HistoryList historyList;

void print_moves(const Move& move) {
    std::string from = index_to_square(move.from_sq);
    std::string to = index_to_square(move.to_sq);

    std::cout << from + to;
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

inline void add_move(MoveList& list, const Move& move) {
    list.moves[list.count++] = move;
}

void gen_psuedo_moves(const full_pos& gamestate, MoveList& list, uint64_t possible_moves, int pos) {
    // If its white to move the knight we are concerned with must be whites 
    // Otherwise its blacks
    if (gamestate.to_move) {
        possible_moves &= ~gamestate.bitboard[white_pieces];
    }
    else {
        possible_moves &= ~gamestate.bitboard[black_pieces];
    }
    //print_bitboard(possible_moves);

    // if moves availble add to move array until it ends
    while (possible_moves) {
        int current_square = get_lsb_index(possible_moves);
        possible_moves &= possible_moves - 1;

        Move current_move;
        current_move.from_sq = pos;
        current_move.to_sq = current_square;
        add_move(list, current_move);
    }
}


bool is_sq_attacked(const full_pos& gamestate, int king_pos, int side_to_move) {
    bool white = side_to_move;

    if (white) {

        if (pawn_attacks[WHITE][king_pos] & gamestate.bitboard[black_pawns]) return true;
        if (knight_attacks[king_pos] & gamestate.bitboard[black_knights]) return true;
        if (king_attacks[king_pos] & gamestate.bitboard[black_king]) return true;
    
    } else {

        if (pawn_attacks[BLACK][king_pos] & gamestate.bitboard[white_pawns]) return true;
        if (knight_attacks[king_pos] & gamestate.bitboard[white_knights]) return true;
        if (king_attacks[king_pos] & gamestate.bitboard[white_king]) return true;
    
    }

    // BISHOP ATTACKS
    uint64_t Bmask = bishop_masks[king_pos];
    uint64_t Bocc = gamestate.bitboard[both_pieces] & Bmask;
    int Bindex = ((Bocc * bishop_magics[king_pos]) >> (64-bishop_shifts[king_pos]));
    uint64_t Battacks = bishop_attacks[king_pos][Bindex];


    if (white){
        if (Battacks & (gamestate.bitboard[black_bishops] | gamestate.bitboard[black_queens])) return true;
    } else {
        if (Battacks & (gamestate.bitboard[white_bishops] | gamestate.bitboard[white_queens])) return true;
    }

    // ROOK ATTACKS
    uint64_t Rmask = rook_masks[king_pos];
    uint64_t Rocc = gamestate.bitboard[both_pieces] & Rmask;
    int Rindex = ((Rocc * rook_magics[king_pos]) >> (64-rook_shifts[king_pos]));
    uint64_t Rattacks = rook_attacks[king_pos][Rindex];

    if (white) {
        if (Rattacks & (gamestate.bitboard[black_rooks] | gamestate.bitboard[black_queens])) return true;
    } else {
        if (Rattacks & (gamestate.bitboard[white_rooks] | gamestate.bitboard[white_queens])) return true;
    }

    return false;
}


void knight_moves(const full_pos& gamestate, MoveList& list, int pos) {
    uint64_t possible_moves = knight_attacks[pos];

    gen_psuedo_moves(gamestate, list, possible_moves, pos);
}

void king_moves(const full_pos& gamestate, MoveList& list, int pos) {
    uint64_t possible_moves = king_attacks[pos];
    //print_bitboard(possible_moves);

    gen_psuedo_moves(gamestate, list, possible_moves, pos);
}

void bishop_moves(const full_pos& gamestate, MoveList& list, int pos) {
    uint64_t mask = bishop_masks[pos];
    uint64_t occ = gamestate.bitboard[both_pieces] & mask;
    int index = ((occ * bishop_magics[pos]) >> (64-bishop_shifts[pos]));

    uint64_t possible_moves = bishop_attacks[pos][index];
    gen_psuedo_moves(gamestate, list, possible_moves, pos);
}

void rook_moves(const full_pos& gamestate, MoveList& list, int pos) {
    uint64_t mask = rook_masks[pos];
    uint64_t occ = gamestate.bitboard[both_pieces] & mask;
    int index = ((occ * rook_magics[pos]) >> (64-rook_shifts[pos]));

    uint64_t possible_moves = rook_attacks[pos][index];
    gen_psuedo_moves(gamestate, list, possible_moves, pos);
}

void queen_moves(const full_pos& gamestate, MoveList& list, int pos) {
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
    gen_psuedo_moves(gamestate, list, possible_queen_moves, pos);
}



// Could refactor these into helper functions nothing too crazy just 
// Popping bits and getting their index and abstracting away make move logic with
// Default parameters, forgot they existed to be honest

void white_pawn_moves(const full_pos& gamestate, MoveList& list) {

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
        add_move(list, move);

        quiet_single_push &= quiet_single_push - 1;
    }

    while (double_push) {

        Move move;
        move.to_sq = get_lsb_index(double_push);
        move.from_sq = move.to_sq + 16;
        move.double_push = true;
        add_move(list, move);

        double_push &= double_push - 1;
    }

    while (promotions) {

        Move move;
        move.to_sq = get_lsb_index(promotions);
        move.from_sq = move.to_sq + 8;
        for (auto promo_piece : {white_queens, white_knights, white_bishops, white_rooks}) {
            move.promotion = promo_piece;
            add_move(list, move);
        }

        promotions &= promotions - 1;

    }

// ------- Handle captures only ---------
    while (whitepawns) {
        int from = get_lsb_index(whitepawns);
        uint64_t attacks = pawn_attacks[WHITE][from] & blackpieces;

        while (attacks) {
            int to = get_lsb_index(attacks);
            Move move;
            move.from_sq = from;
            move.to_sq = to;

            // check for promotion 
            if ((1ULL << to) & rank_8) {
                for (auto promo_piece : {white_queens, white_knights, white_bishops, white_rooks}) {
                    move.promotion = promo_piece;
                    add_move(list, move);
                }
            } else {
                add_move(list, move);
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
        int ep_rank = ep + 8; 

        while (whitepawns) {
            int from = get_lsb_index(whitepawns);
            if (pawn_attacks[WHITE][from] & ep_bb && (gamestate.bitboard[black_pawns] & 1ULL << ep_rank)) {

                Move move;
                move.from_sq = from;
                move.to_sq = ep;
                move.ep = true;

                add_move(list, move);
            }
            whitepawns &= whitepawns - 1;
        }
    }

}



void black_pawn_moves(const full_pos& gamestate, MoveList& list) {
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
        add_move(list, move);

        quiet_single_push &= quiet_single_push - 1;
    }

    while (double_push) {

        Move move;
        move.to_sq = get_lsb_index(double_push);
        move.from_sq = move.to_sq - 16;
        move.double_push = true;
        add_move(list, move);

        double_push &= double_push - 1;
    }

    while (promotions) {

        Move move;
        move.to_sq = get_lsb_index(promotions);
        move.from_sq = move.to_sq - 8;
        for (auto promo_piece : {black_queens, black_knights, black_bishops, black_rooks}) {
            move.promotion = promo_piece;
            add_move(list, move);
        }

        promotions &= promotions - 1;

    }

// ------- Handle captures only ---------
    while (blackpawns) {
        int from = get_lsb_index(blackpawns);
        uint64_t attacks = pawn_attacks[BLACK][from] & whitepieces;

        while (attacks) {
            int to = get_lsb_index(attacks);
            Move move;
            move.from_sq = from;
            move.to_sq = to;

            // check for promotion 
            if ((1ULL << to) & rank_1) {
                for (auto promo_piece : {black_queens, black_knights, black_bishops, black_rooks}) {
                    move.promotion = promo_piece;
                    add_move(list, move);
                }
            } else {
                add_move(list, move);
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
        int ep_rank = ep - 8;

        while (blackpawns) {
            int from = get_lsb_index(blackpawns);
            if (pawn_attacks[BLACK][from] & ep_bb && (gamestate.bitboard[white_pawns] & 1ULL << ep_rank)) {

                Move move;
                move.from_sq = from;
                move.to_sq = ep;
                move.ep = true;

                add_move(list, move);
            }
            blackpawns &= blackpawns - 1;
        }
    }

}



void castling(const full_pos& gamestate, MoveList& list) {

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
        add_move(list, move);

    }

    if (gamestate.white_queen_side_castle && Wqueenside_clear) {

        Move move;
        move.from_sq = e1;
        move.to_sq = c1;
        move.castle = true;
        add_move(list, move);

    }

    // Black Castling
    if (gamestate.black_king_side_castle && Bkingside_clear) {

        Move move;
        move.from_sq = e8;
        move.to_sq = g8;
        move.castle = true;
        add_move(list, move);

    }

    if (gamestate.black_queen_side_castle && Bqueenside_clear) {

        Move move;
        move.from_sq = e8;
        move.to_sq = c8;
        move.castle = true;
        add_move(list, move);

    }
}




// Once confirmed correct refactor all of this into a more cleaner code 
template <typename Func>
void append_moves_from_bitboard(MoveList& all_moves, const full_pos& gamestate, uint64_t bitboard, Func move_generator) {
    while (bitboard) {
        int pos = get_lsb_index(bitboard);
        bitboard &= bitboard - 1; // clear LSB

        move_generator(gamestate, all_moves, pos);
    }
}



MoveList psuedo_moves(const full_pos& gamestate) {
    MoveList all_moves;

    if (gamestate.to_move) {

        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[white_knights], knight_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[white_bishops], bishop_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[white_rooks], rook_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[white_queens], queen_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[white_king], king_moves);
        white_pawn_moves(gamestate, all_moves);

    } else {

        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[black_knights], knight_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[black_bishops], bishop_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[black_rooks], rook_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[black_queens], queen_moves);
        append_moves_from_bitboard(all_moves, gamestate, gamestate.bitboard[black_king], king_moves);
        black_pawn_moves(gamestate, all_moves);

    }

    castling(gamestate, all_moves);

    return all_moves;
}



inline void recompute_aggregates(full_pos& g) {
    uint64_t w = 0, b = 0;
    for (int p = white_pawns; p <= white_king; ++p) w |= g.bitboard[p];
    for (int p = black_pawns; p <= black_king; ++p) b |= g.bitboard[p];
    g.bitboard[white_pieces] = w;
    g.bitboard[black_pieces] = b;
    g.bitboard[both_pieces] = w | b;
}


void clear_piece(full_pos& state, int piece_clear, int sq_clear) {
    pop_bit(state.bitboard[piece_clear], sq_clear);
    pop_bit(state.bitboard[both_pieces], sq_clear);
    state.piece_map[sq_clear] = empty;
    if (piece_clear <= white_king){
        pop_bit(state.bitboard[white_pieces], sq_clear);
    } else {
        pop_bit(state.bitboard[black_pieces], sq_clear);
    }
} 

void move_piece(full_pos& state, int from, int to, int piece_moving) {
    clear_piece(state, piece_moving, from);
    set_bit(state.bitboard[piece_moving], to);
    set_bit(state.bitboard[both_pieces], to);
    state.piece_map[to] = piece_moving;
    if (piece_moving <= white_king){
        set_bit(state.bitboard[white_pieces], to);
    } else {
        set_bit(state.bitboard[black_pieces], to);
    }
}

void put_piece(full_pos& state, int piece, int sq){
    set_bit(state.bitboard[piece], sq);
    set_bit(state.bitboard[both_pieces], sq);
    state.piece_map[sq] = piece;
    if (piece <= white_king){
        set_bit(state.bitboard[white_pieces], sq);
    } else {
        set_bit(state.bitboard[black_pieces], sq);
    }
}


void do_castle(full_pos& state, int Kfrom, int Kto, int Rfrom, int Rto) {
    int king = state.piece_map[Kfrom];
    int rook = state.piece_map[Rfrom];

    // Move King to square
    move_piece(state, Kfrom, Kto, king);

    // Move Rook to square
    move_piece(state, Rfrom, Rto, rook);
}


void make_move(full_pos& state, Move& move) {

    int from = move.from_sq;
    int to = move.to_sq;
    int mover_piece = state.piece_map[from];

    // Undo flags, all are intialised to prev state or empty where applicible
    Undo undo;
    undo.move = move;

    undo.captured_piece = empty;
    undo.captured_square = empty;
    undo.mover_piece = mover_piece;

    undo.Wkingside = state.white_king_side_castle;
    undo.WQueenside = state.white_queen_side_castle;
    undo.BKingside = state.black_king_side_castle;
    undo.BQueenside = state.black_queen_side_castle;

    undo.old_ep_square = state.enpassant_square;
    undo.old_half_move = state.half_move_clock;
    undo.old_full_move = state.full_move_clock;


    // ---- Special Cases -----
    // Making Moves here 


    // Enpassant Case
    
    if (move.ep) {
        if (state.to_move) {
            
            // Set sqauare and piece to be captured
            int captured_sqaure = to + 8;
            int captured_piece = state.piece_map[captured_sqaure];
            
            // Clear captured pawn
            clear_piece(state, captured_piece, captured_sqaure);
        
            // Move freindly pawn
            move_piece(state, from, to, mover_piece);

            // Save undo info 
            undo.captured_piece = captured_piece;
            undo.captured_square = captured_sqaure;

        } else {

            // Set sqauare and piece to be captured
            int captured_sqaure = to - 8;
            int captured_piece = state.piece_map[captured_sqaure];
            
            // Clear captured pawn
            clear_piece(state, captured_piece, captured_sqaure);
        
            // Move freindly pawn
            move_piece(state, from, to, mover_piece);

            // Save undo info 
            undo.captured_piece = captured_piece;
            undo.captured_square = captured_sqaure;

        }

    }


    // Promotion
    else if (move.promotion) {
        // Stored the mover piece in the undo info
        int captured_square = to;
        int captured_piece = state.piece_map[captured_square];

        // Clear the pawn from the sqaure its moving from
        clear_piece(state, mover_piece, from);

        if (captured_piece != empty) {
            // Save captured undo info
            undo.captured_piece = captured_piece;
            undo.captured_square = captured_square;

            // Clear the captured piece
            clear_piece(state, captured_piece, captured_square);
        }

        // Promote the pawn
        put_piece(state, move.promotion, to);

    }


    // Castling
    // Could abstract away reused logic but for correctness will do later
    else if (move.castle) {
        if (to == g1) {

            // King start and end squares
            int king_from = from;
            int king_to = to;

            // Rook start and end squares
            int rook_from = h1;
            int rook_to = f1;

            // Move pieces 
            do_castle(state, king_from, king_to, rook_from, rook_to);

            // Disable white's right to castle
            state.white_king_side_castle = false;
            state.white_queen_side_castle = false;
        }

        else if (to == c1) {

            // King start and end squares
            int king_from = from;
            int king_to = to;

            // Rook start and end squares
            int rook_from = a1;
            int rook_to = d1;

            // Move pieces 
            do_castle(state, king_from, king_to, rook_from, rook_to);

            // Disable white's right to castle
            state.white_king_side_castle = false;
            state.white_queen_side_castle = false;
        }


        else if (to ==  g8) {
            // King start and end squares
            int king_from = from;
            int king_to = to;

            // Rook start and end squares
            int rook_from = h8;
            int rook_to = f8;

            // Move pieces 
            do_castle(state, king_from, king_to, rook_from, rook_to);

            // Disable black's right to castle
            state.black_king_side_castle = false;
            state.black_queen_side_castle = false;
        }

        else if (to == c8) {
            // King start and end squares
            int king_from = from;
            int king_to = to;

            // Rook start and end squares
            int rook_from = a8;
            int rook_to = d8;

            // Move pieces 
            do_castle(state, king_from, king_to, rook_from, rook_to);

            // Disable black's right to castle
            state.black_king_side_castle = false;
            state.black_queen_side_castle = false;
        }
    }


    // ---- Normal case ----
    else {
        int captured_square = to;
        int captured_piece = state.piece_map[to];

        if (captured_piece != empty) {

            // Clear captured piece off the board 
            clear_piece(state, captured_piece, captured_square);

            // Save undo captured info
            undo.captured_piece = captured_piece;
            undo.captured_square = captured_square;
        }

        // Move piece to the dest square
        move_piece(state, from, to, mover_piece);

    }

    // Loss of castling rights
    if (!move.castle) {
        if (mover_piece == white_rooks) {
            if (from == a1) state.white_queen_side_castle = false;
            if (from == h1) state.white_king_side_castle  = false;
        }
        if (mover_piece == black_rooks) {
            if (from == a8) state.black_queen_side_castle = false;
            if (from == h8) state.black_king_side_castle  = false;
        }
        if (mover_piece == white_king) {
            state.white_queen_side_castle = false;
            state.white_king_side_castle  = false;
            state.white_king_pos = get_lsb_index(state.bitboard[white_king]);
        }
        if (mover_piece == black_king) {
            state.black_queen_side_castle = false;
            state.black_king_side_castle  = false;
            state.black_king_pos = get_lsb_index(state.bitboard[black_king]);

        }

        // Also handle rook captured on original square:
        if (undo.captured_piece == white_rooks) {
            if (undo.captured_square == a1) state.white_queen_side_castle = false;
            if (undo.captured_square == h1) state.white_king_side_castle  = false;
        }
        if (undo.captured_piece == black_rooks) {
            if (undo.captured_square == a8) state.black_queen_side_castle = false;
            if (undo.captured_square == h8) state.black_king_side_castle  = false;
        }
    }


    // Set half move clock
    bool is_pawn = (mover_piece == white_pawns || mover_piece == black_pawns);
    if (is_pawn || undo.captured_piece != empty) {
        state.half_move_clock = 0;
    } else {
        state.half_move_clock++;
    }

    // Set enpassant square
    if (move.double_push) {
        state.enpassant_square = (state.to_move) ? to + 8 : to - 8;
    } else {
        state.enpassant_square = empty;
    }

    // If black turn to move increment move clock
    if (!state.to_move) {
        state.full_move_clock++;
    }

    if (mover_piece == white_king) {
        state.white_king_pos = get_lsb_index(state.bitboard[white_king]);
    }
    else if (mover_piece == black_king) {
        state.black_king_pos = get_lsb_index(state.bitboard[black_king]);
    }

    // Switch side to move
    state.to_move = !state.to_move;

    historyList.history[historyList.count++] = undo;
}

void unmake_move(full_pos& state) {
    // Restore flags
    Undo undo = historyList.history[--historyList.count];

    // Flip side to move
    state.to_move = !state.to_move;

    Move move = undo.move;

    state.white_king_side_castle = undo.Wkingside;
    state.white_queen_side_castle = undo.WQueenside;
    state.black_king_side_castle = undo.BKingside;
    state.black_queen_side_castle = undo.BQueenside;

    state.enpassant_square = undo.old_ep_square;
    state.half_move_clock = undo.old_half_move;
    state.full_move_clock = undo.old_full_move;

    // what piece to move back
    int mover_piece = undo.mover_piece;

     
    int from = move.from_sq;
    int to = move.to_sq;


    // --- Special Cases ----

    // Enpassant
    if (move.ep) {
        int captured_square = undo.captured_square;
        int captured_piece = undo.captured_piece;
        // Move pawn back before enpassant
        move_piece(state, to, from, mover_piece);

        // Put the oppenent pawn back
        put_piece(state, captured_piece, captured_square);
    }

    // Promotions
    else if (move.promotion) {
        int promoted_piece = state.piece_map[to];
        int promoted_square = to;
        int pawn = undo.mover_piece;

        // Clear promoted piece
        clear_piece(state, promoted_piece, promoted_square);


        // Capture promotion?
        if (undo.captured_piece != empty){
            // Set captured sqaure and piece
            int captured_piece = undo.captured_piece;
            int captured_square = undo.captured_square;

            // Restore captured piece
            put_piece(state, captured_piece, captured_square);
        }

        // Restore pawn
        put_piece(state, pawn, from);

    }

    // Castling 
    else if (move.castle) {

        if (to == g1) {
            // Move king back 
            move_piece(state, to, from, mover_piece);

            // Move rook back
            int rook = state.piece_map[f1];
            move_piece(state, f1, h1, rook);
        }

        else if (to == c1) {
            // Move king back 
            move_piece(state, to, from, mover_piece);

            // Move rook back
            int rook = state.piece_map[d1];
            move_piece(state, d1, a1, rook);
        }

        else if (to == g8) {
            // Move king back 
            move_piece(state, to, from, mover_piece);

            // Move rook back
            int rook = state.piece_map[f8];
            move_piece(state, f8, h8, rook);
        }

        else if (to == c8) {
            // Move king back 
            move_piece(state, to, from, mover_piece);

            // Move rook back
            int rook = state.piece_map[d8];
            move_piece(state, d8, a8, rook);
        }  
    }

    // ---- Normal move -----
    else {
        // Move piece back 
        move_piece(state, to, from, mover_piece);

        // Captured a piece? Restore it 
        if (undo.captured_piece != empty) {
            put_piece(state, undo.captured_piece, undo.captured_square);
        }
    }

    if (mover_piece == white_king) {
        state.white_king_pos = get_lsb_index(state.bitboard[white_king]);
    }
    else if (mover_piece == black_king) {
        state.black_king_pos = get_lsb_index(state.bitboard[black_king]);
    }
}

MoveList legal_move_gen(full_pos &state) {
    MoveList legal_moves;

    int king_pos = state.to_move ? state.white_king_pos : state.black_king_pos;
    MoveList psuedo_legal = psuedo_moves(state);

    for (int i = 0; i < psuedo_legal.count; i++) {
        Move move = psuedo_legal.moves[i];
        
        if (move.castle) { 

            if (is_sq_attacked(state, king_pos, state.to_move)) {
                continue; 
            }

            if (move.to_sq == g1 && (is_sq_attacked(state, f1, state.to_move) || is_sq_attacked(state, g1, state.to_move))) {
                continue;
            }

            if (move.to_sq == c1 && (is_sq_attacked(state, e1, state.to_move) || is_sq_attacked(state, d1, state.to_move) || is_sq_attacked(state, c1, state.to_move))) {
                continue;
            }

            if (move.to_sq == g8 && (is_sq_attacked(state, f8, state.to_move) || is_sq_attacked(state, g8, state.to_move))) {
                continue;
            }

            if (move.to_sq == c8 && (is_sq_attacked(state, e8, state.to_move) || is_sq_attacked(state, d8, state.to_move) || is_sq_attacked(state, c8, state.to_move))) {
                continue;
            }
        }

        // ---- Normal moves -----
        make_move(state, move);
        int king_pos = !state.to_move ? state.white_king_pos : state.black_king_pos;
        if (is_sq_attacked(state, king_pos, !state.to_move)) {
            unmake_move(state); 
            continue;
        }
        else {
            add_move(legal_moves, move);
            unmake_move(state);
        }
    }

    return legal_moves;
}

