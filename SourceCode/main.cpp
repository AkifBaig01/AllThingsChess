#include "uci.h"
#include "bitboard.h"
#include "utils.h"
#include "movegen.h"
#include "moves.h"
#include "evaluate.h"

#include "uci_real.cpp"

#include <iostream>
#include <string>

uint64_t perft_test_make_unmake(full_pos &pos, int depth) {
    if (depth == 0) return 1;

    auto moves = legal_move_gen(pos);
    uint64_t nodes = 0;

    // Only print at root (when starting depth == depth passed in)
    static int start_depth = -1;
    if (start_depth == -1) start_depth = depth;

    for (auto m : moves) {
        make_move(pos, m);
        uint64_t child_nodes = perft_test_make_unmake(pos, depth - 1);
        unmake_move(pos);

        nodes += child_nodes;

        // Print only at root depth
        if (depth == start_depth) {
            print_moves(m);
            std::cout  << " : " << child_nodes << std::endl;
        }
    }

    if (depth == start_depth) {
        std::cout << "Total nodes: " << nodes << std::endl;
        start_depth = -1; // reset for next perft run
    }

    return nodes;
}

// Print the current board postion represented in memory
// May need to improve slightly
void print_represent(const full_pos& represent) {
        for (int i = 0; i < piece_no; i++){
        print_bitboard(represent.bitboard[i]);
    }

    std::cout << "WHITE KING SIDE " << represent.white_king_side_castle << '\n';
    std::cout << "WHITE QUEEN SIDE " << represent.white_queen_side_castle << '\n';
    std::cout << "BLACK KING SIDE " << represent.black_king_side_castle << '\n';
    std::cout << "BLACK QUEEN SIDE " << represent.black_queen_side_castle << '\n';

    std::cout << "EP SQUARE " << represent.enpassant_square << '\n';

    std::cout << "HALF MOVE " << represent.half_move_clock<< '\n';

    std::cout << "FULL MOVE " << represent.full_move_clock << '\n';

    std::cout << "TO MOVE " << represent.to_move << '\n';
    
    std::cout << "This is the end of this section \n\n\n\n";
}


int main() {
    
    
    std::string fen = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";

    fen_rep board = fen_parser(fen); 

    full_pos represent = set_bitboard_pos(board);

    print_represent(represent);

    init_all_attacks();

    int depth = 6; // start with 3 or 4 for good coverage
    std::cout << "Running perft_test_make_unmake depth " << depth << "...\n";
    uint64_t nodes = perft_test_make_unmake(represent, depth);
    std::cout << "Completed. Nodes: " << nodes << '\n';

    
    auto moves = legal_move_gen(represent);
    std::cout << moves.size() << std::endl;

    for (auto move: moves) {
    }
    if (game_ended(represent, moves) == NONE) {
        std::cout << "Game is Playable :" << std::endl;
    } else {
        std::cout << "Game is not playable : " << game_ended(represent, moves) << std:: endl;
    }

    //std::cout << evaluate(represent) << std::endl;

    //print_represent(represent);

    //search(6, represent);

    
   
    // init_all_attacks();
    //uci_loop();

    return 0;
}