#include <utils.h>
#include <uci.h>

#include <cstdint>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <bitset>
#include <cctype>
#include <algorithm>
#include <vector>

// ALL DECLARATIONS

// Return piece calues based on enum declartions
int get_piece_enum(char c) {

    // Jump tables, saves multiple if statements 
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
        
        default: return empty;
    }
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

    std::cout << "\n";
    std::cout << "This is to move " << parsed.to_move << "\n";
    

    std::cout << "\n";
    std::cout << "This is for white king side castle " << parsed.white_king_side_castle << "\n";
    std::cout << "This is for white queen side castle " << parsed.white_queen_side_castle << "\n";
    std::cout << "This is for black king side castle " << parsed.black_king_side_castle << "\n";
    std::cout << "This is for black queen side castle " << parsed.black_queen_side_castle << "\n";
    std::cout << "\n";

    std::cout << "This is for enpassant " << parsed.enpassant_square << "\n";
    std::cout << "This is for halfmove clock "  << parsed.half_move_clock << "\n";
    std::cout << "This is for fullmove clock " << parsed.full_move_clock  << "\n";


}

// ALL DECLARATIONS





// WORKING PROGRAM

fen_rep board_pos(fen_rep& parsed, std::string fen) {
    // Intialise an object of the defined struct 
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
            // did not put i += 1 seems to be working dont know why
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



fen_rep to_move(fen_rep& parsed, std::string turn_move) {
    if (turn_move == "w") {
        parsed.to_move = white;
    }
    else
        parsed.to_move = black;

    return parsed;
}

fen_rep enpassant(fen_rep& parsed, std::string enpassant_square) {
    if (enpassant_square == "-") {
        parsed.enpassant_square = -1;
    }
    else {
        int current_enpssant_sqaure = sqaure_to_index(enpassant_square);
        parsed.enpassant_square = current_enpssant_sqaure;
    }
    
    return parsed;
}

fen_rep castling(fen_rep& parsed, std::string castling_rights) {
    // Goes through string to find if char exists
    // if it does returns index where it exists 
    // otherwise returns npos menaing "not found"

    // white rights
    parsed.white_king_side_castle = castling_rights.find('K') != std::string::npos;
    parsed.white_queen_side_castle = castling_rights.find('Q') != std::string::npos;
    
    // black rights
    parsed.black_king_side_castle = castling_rights.find('k') != std::string::npos;
    parsed.black_queen_side_castle = castling_rights.find('q') != std::string::npos;

    return parsed;
}


fen_rep half_move (fen_rep& parsed, std::string halfmove) {
    parsed.half_move_clock = std::stoi(halfmove);

    return parsed;
}

fen_rep full_moves (fen_rep& parsed, std::string fullmoves) {
    parsed.full_move_clock = std::stoi(fullmoves);

    return parsed;
}


fen_rep more_moves (fen_rep& parsed, std::vector<std::string> moremoves) {
    for (std::string move : moremoves) {
        int from = sqaure_to_index(move.substr(0,2));
        int temp = parsed.squares[from];
        parsed.squares[from] = empty;
        int to = sqaure_to_index(move.substr(2,2));
        parsed.squares[to] = temp;
    }
    return parsed; 
}


// add in split purely one function call MORE CONSCISE
fen_rep fen_parser(std::string fen_string) {
    fen_rep board;
    std::vector<std::string> fen_array = split(fen_string);
    // Main parser call other helper function 
    // Aim is to call this one fucntion to setup the final strcut to be passed onto bitbaord.cpp
    // Want to call the list of functions in an array like python would make this alot cleaer and nicer
    if (fen_array.size() <= 6) {
        board = board_pos(board, fen_array[0]);
        board = to_move(board, fen_array[1]);
        board = castling(board, fen_array[2]);
        board = enpassant(board, fen_array[3]);
        board = half_move(board, fen_array[4]);
        board = full_moves(board, fen_array[5]);
    }

    else {
        board = board_pos(board, fen_array[0]);
        board = to_move(board, fen_array[1]);
        board = castling(board, fen_array[2]);
        board = enpassant(board, fen_array[3]);
        board = half_move(board, fen_array[4]);
        board = full_moves(board, fen_array[5]);
        std::vector<std::string> moves(fen_array.begin() + 7, fen_array.end());
        for (std::string move : moves) {
            std::cout << move << '\n';
        }
        board = more_moves(board, moves);
    }
    
    return board;
}


// WORKING PROGRAM 


/*
LIMITATIONS:
A partial move gen has been added but does NOT update state accurately
Only the use of the position fen strign along with board elemnts is considered functional
Move gen and legal move checking will be added later I fear it is outside the scope of this file 
and its respnsibilities

next steps are to refactor simplify comment and integrate this file with the rest of the engine

*/