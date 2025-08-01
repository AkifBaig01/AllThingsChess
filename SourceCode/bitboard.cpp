#include "bitboard.h"
#include "uci.h"
#include "utils.h"

#include <cstdint>
#include <iostream>
#include <stdio.h>

// Few macros just to aid readability
#define WHITE_PIECES (full_position.bitboard[white_pawns] | full_position.bitboard[white_knights] | full_position.bitboard[white_bishops] | full_position.bitboard[white_rooks] | full_position.bitboard[white_queens] | full_position.bitboard[white_king])
#define BLACK_PIECES (full_position.bitboard[black_pawns] | full_position.bitboard[black_knights] | full_position.bitboard[black_bishops] | full_position.bitboard[black_rooks] | full_position.bitboard[black_queens] | full_position.bitboard[black_king])

// Function takes reference to fen_rep struct and simply updates
// Its own bitboards
full_pos set_bitboard_pos(fen_rep& fen_pos) {
    // Intialise struct to build full postition
    full_pos full_position;

    // Build bitboards
    int i = 0;
    for (int piece : fen_pos.squares) {
        if (piece != empty) {
            set_bit(full_position.bitboard[piece], i);
        }
    i += 1;
    }

    // Allocate piece map
    for (int j = 0; j < 64; j++) {
        full_position.piece_map[j] = fen_pos.squares[j];
    }

    // White Castling
    full_position.white_king_side_castle = fen_pos.white_king_side_castle;
    full_position.white_queen_side_castle = fen_pos.white_queen_side_castle;

    // Black Castling
    full_position.black_king_side_castle = fen_pos.black_king_side_castle;
    full_position.black_queen_side_castle = fen_pos.black_queen_side_castle;

    // Setting the rest of the flags
    full_position.enpassant_square = fen_pos.enpassant_square;
    full_position.half_move_clock = fen_pos.half_move_clock;
    full_position.full_move_clock = fen_pos.full_move_clock;
    full_position.to_move = fen_pos.to_move;

    // Update combined piece representations
    full_position.bitboard[white_pieces] = WHITE_PIECES;
    full_position.bitboard[black_pieces] = BLACK_PIECES;
    full_position.bitboard[both_pieces] = WHITE_PIECES | BLACK_PIECES;
    
    return full_position;
}
