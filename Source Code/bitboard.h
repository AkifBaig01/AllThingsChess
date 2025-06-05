#ifndef BITBOARD_H
#define BITBOARD_H

#include <string>
#include <cstdint>

/*
Struct is similar to class is going to be used to represent the entire board postion.operator delete
It will consists of entries such as 

struct CurrentBoardState {
    array of 12 bitbaords with return type ULL 
    en passant sqaures availible only one sqaure ever per move?
    castling rights KQkq
    white or black to move
}
 Is struct the best structure for this? this is unclear at this moment in the implementation.
*/
struct CurrentBoardState;


/*
This should be the main fucntion which should return the fen parsed and return the current board postion
Unsure if compiler will throw errors for not implementing a main function
*/
CurrentBoardState generate_bitboard_from_fen(const std::string& fen);


// Helper Fucntions
uint64_t generate_pawn_bitboard(const std::string& fen, bool white);
uint64_t generate_kngiht_bitboard(const std::string& fen, bool white);
uint64_t generate_bishop_bitboard(const std::string& fen, bool white);
uint64_t generate_queen_bitboard(const std::string& fen, bool white);
uint64_t generate_king_bitboard(const std::string& fen, bool white);



#endif