#include "evaluate.h"
#include "bitboard.h"
#include "moves.h"
#include "utils.h"

#include <vector>


// helper: flip ranks (A8=0 ... H1=63 indexing)
inline int mirror_square(int sq) { 
    return sq ^ 56; 
}

inline int opponent_of(int side) { 
    return (side == WHITE) ? BLACK : WHITE; 
}

// return 0 for dark, 1 for light
inline int square_color(int sq) {
    return (((sq / 8) + (sq % 8)) & 1);
}

// --- Game end checks ---

bool is_checkmate(const full_pos& state, const std::vector<Move>& legal_moves) {
    if (!legal_moves.empty()) return false;
    int king_pos = (state.to_move == WHITE) ? state.white_king_pos : state.black_king_pos;
    return is_sq_attacked(state, king_pos, state.to_move);
}

bool is_stalemate(const full_pos& state, const std::vector<Move>& legal_moves) {
    if (!legal_moves.empty()) return false;
    int king_pos = (state.to_move == WHITE) ? state.white_king_pos : state.black_king_pos;
    return !is_sq_attacked(state, king_pos, state.to_move);
}

bool is_50_move_rule(const full_pos& state) {
    return state.half_move_clock >= 100;
}


bool is_insufficient_material(const full_pos& board) {
    int white_knight = 0, black_knight = 0;
    int white_bishop = 0, black_bishop = 0;
    int white_other = 0, black_other = 0; // pawns, rooks, queens
    int white_bishop_color = -1, black_bishop_color = -1;

    for (int sq = 0; sq < 64; ++sq) {
        int piece = board.piece_map[sq];
        if (piece == empty) continue;

        switch (piece) {
            // white pawns/rooks/queens => sufficient material immediately
            case white_pawns:
            case white_rooks:
            case white_queens:
                ++white_other;
                break;

            case black_pawns:
            case black_rooks:
            case black_queens:
                ++black_other;
                break;

            case white_knights:
                white_knight++;
                break;

            case black_knights:
                black_knight++;
                break;

            case white_bishops:
                white_bishop++;
                white_bishop_color = square_color(sq);
                break;
            case black_bishops:
                black_bishop++;
                black_bishop_color = square_color(sq);
                break;

            default:
                break;
        }
    }

    // If either side has pawns/rooks/queens => sufficient
    if (white_other > 0 || black_other > 0) return false;

    // K vs K
    if (white_knight == 0 && white_bishop == 0 && black_knight == 0 && black_bishop == 0)
        return true;

    // K+N vs K (single knight vs lone king)
    if ( (white_knight == 1 && white_bishop == 0 && black_knight == 0 && black_bishop == 0) ||
         (black_knight == 1 && black_bishop == 0 && white_knight == 0 && white_bishop == 0) )
        return true;

    // K+B vs K (single bishop vs lone king)
    if ( (white_bishop == 1 && white_knight == 0 && black_bishop == 0 && black_knight == 0) ||
         (black_bishop == 1 && black_knight == 0 && white_bishop == 0 && white_knight == 0) )
        return true;

    // K+B vs K+B (both sides have exactly one bishop AND no knights) and bishops on same color -> draw
    if (white_bishop == 1 && black_bishop == 1 && white_knight == 0 && black_knight == 0) {
        if (white_bishop_color == black_bishop_color)
            return true;
    }

    // All other cases -> sufficient material (e.g., K+2N vs K is NOT insufficient)
    return false;
}

// Start with out high level concept
int game_ended(const full_pos& state, const std::vector<Move>& legal_moves) {
    if (is_checkmate(state, legal_moves)) return CHECKMATE;

    if (is_stalemate(state, legal_moves)) return STALEMATE;

    if (is_insufficient_material(state)) return INSUFMAT;

    if (is_50_move_rule(state)) return FIFTYMOVE;

    //if (is_threefold()) return true;

    return NONE;
}

// Start static evaluation 
int material_count(const full_pos& state) {
    int score = 0;

    for (int sq = 0; sq < 64; sq++) {
        int piece = state.piece_map[sq];

        if (piece == empty) continue;

        int type = piece_type(piece);
        int colour = piece_colour(piece);

        int base = centipawn[type];
        // Using not as make move will be called first which will flip side to move
        int psq_index = (colour == WHITE ? sq : mirror_square(sq));

        int psq_bonus = 0;
        switch (type) {
            case PAWN:   psq_bonus = pawn_psqt[psq_index]; break;
            case KNIGHT:   psq_bonus = knight_psqt[psq_index]; break;
            case BISHOP:   psq_bonus = bishop_psqt[psq_index]; break;
            case ROOK:   psq_bonus = rook_psqt[psq_index]; break;
            case QUEEN:   psq_bonus = queen_psqt[psq_index]; break;
            case KING:   psq_bonus = king_psqt[psq_index]; break;
        }

        score += (colour == WHITE ? +1 : -1) * (base + psq_bonus);

    }

    return score;
}


int evaluate(const full_pos& state) {
    int score  = material_count(state);

    return score;
}