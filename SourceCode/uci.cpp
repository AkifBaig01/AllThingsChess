#include <cstdint>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <bitset>
#include <cctype>
#include <algorithm>

/*
Parsed fen will be stored in a struct and that struct will be passed onto bitboard.cpp
*/
 struct fen_rep
 {
    int squares[64];  // hold the values for board sqaures occupied and by what piece

    // Castling rights explicitly deifned 
    bool white_king_side_castle = false;
    bool white_queen_side_castle = false;
    bool black_king_side_castle = false;
    bool black_queen_side_castle = false;
    
    int enpassant_square = -1;  // -1 if not availible otherwise square index
    int half_move_clock = 0; // keep track of the 50 move rule 
    int full_move_clock = 1; // full move numbers incremented after black to move

    int to_move = 1; // 1 - white to move, 0 - black to move

    fen_rep() {
        std::fill(squares, squares + 64, -1);
    }
 };


// For piece case tables and indexing for array
enum {
    white_pawns, white_knights, white_bishops, white_rooks, white_queens, white_king,
    black_pawns, black_knights, black_bishops, black_rooks, black_queens, black_king,
    piece_no
};



int get_piece_enum(char c) {
    switch (c) { 

        // black pieces
        case 'p': return black_pawns;
        case 'n': return black_knights;
        case 'b': return black_bishops;
        case 'r': return black_rooks;
        case 'q': return black_queens;
        case 'k': return black_king;

        // white pieces 
        case 'P': return white_pawns;
        case 'N': return white_knights;
        case 'B': return white_bishops;
        case 'R': return white_rooks;
        case 'Q': return white_queens;
        case 'K': return white_king;
        
        default: return -1;
    }
}

fen_rep parser(std::string fen) {
    // Intialise an object of the defined struct 
    fen_rep parsed;
    int i = 0;

    // Loop through each board square
    for (char c : fen) {

        // Check if isdigit if so simply skip
        if (std::isdigit(static_cast<unsigned char>(c))){

            // Convert to string not char and add
            std::string s(1,c);
            int num = std::stoi(s);

            i += num;
        }

        // Start of new rank just continue 
        else if (c == '/') {
            continue;
        }

        // Real pieces call  and obtain the correct values 
        else {
            parsed.squares[i] = get_piece_enum(c);
            i += 1;
        }
    }

    return parsed;
}


void helper_print(fen_rep& parsed) {
        for (int rank = 0; rank <8; rank++) {
        // Loop over board files
            for (int file = 0; file < 8; file++) {
                // convert file and rank into sqaure index
                int sqaure = rank * 8 + file;
                
                // print ranks 
                if (!file)
                    printf("%d - ", 8 - rank);
                
                printf("%d ", parsed.squares[sqaure]);
            }

        // Print new line every rank 
        printf("\n");
    }
}



/*
int main() {
    std::string fen = "8/8/8/3p1b2/8/4N3/2R1P1P1/4Q1k1";
    fen_rep board = parser(fen);
    helper_print(board);

    return 0;
}
*/