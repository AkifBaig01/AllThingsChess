#include "uci.h"
#include "bitboard.h"
#include "utils.h"
#include "movegen.h"
#include "moves.h"
#include "evaluate.h"


#include <iostream>
#include <string>
#include <cassert>


void test_make_unmake(full_pos& pos, Move m) {
    full_pos backup = pos; // simple struct copy
    //print_moves(m);

    make_move(pos, m);
    unmake_move(pos);

    for (int i = 0; i < piece_no; ++i) {
        if (pos.bitboard[i] != backup.bitboard[i]) {
            std::cout << "Failed on bitboard[" << i << "]\n";
            assert(false);
        }
    }

    for (int i = 0; i < 64; ++i) {
        if (pos.piece_map[i] != backup.piece_map[i]) {
            std::cout << "Failed on piece_map[" << i << "]\n";
            assert(false);
        }
    }

    if (pos.white_king_side_castle != backup.white_king_side_castle) {
        std::cout << "Failed on white_king_side_castle\n";
        assert(false);
    }
    if (pos.white_queen_side_castle != backup.white_queen_side_castle) {
        std::cout << "Failed on white_queen_side_castle\n";
        assert(false);
    }
    if (pos.black_king_side_castle != backup.black_king_side_castle) {
        std::cout << "Failed on black_king_side_castle\n";
        assert(false);
    }
    if (pos.black_queen_side_castle != backup.black_queen_side_castle) {
        std::cout << "Failed on black_queen_side_castle\n";
        assert(false);
    }

    if (pos.enpassant_square != backup.enpassant_square) {
        std::cout << "Failed on enpassant_square\n";
        assert(false);
    }
    if (pos.half_move_clock != backup.half_move_clock) {
        std::cout << "Failed on half_move_clock\n";
        assert(false);
    }
    if (pos.full_move_clock != backup.full_move_clock) {
        std::cout << "Failed on full_move_clock\n";
        assert(false);
    }
    if (pos.to_move != backup.to_move) {
        std::cout << "Failed on to_move\n";
        assert(false);
    }
}

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
    
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    fen_rep board = fen_parser(fen); 

    full_pos represent = set_bitboard_pos(board);

    print_represent(represent);

    init_all_attacks();
    
    //print_bitboard(bishop_attacks[d5][(test * bishop_magics[d5]) >> bishop_shifts[d5]]);

    /*
    std::vector<Move> psuedo_white_pawn = psuedo_moves(represent);
    int total_moves = 0;

    for (Move move : psuedo_white_pawn) {
        test_make_unmake(represent, move);
        total_moves++;
    }
    
    std::cout << total_moves << std::endl;
    */

    uint64_t test = 0ULL;

    set_bit(test, a1);
    set_bit(test, b1);
    set_bit(test, c1);
    set_bit(test, d1);
    set_bit(test, e1);
    set_bit(test, f1);
    set_bit(test, g1);
    set_bit(test, h1);

    //if (is_sq_attacked(represent, e1)) std::cout << "1 I am attacked" << std::endl;
    //else std::cout << "0 Not attacked" << std::endl;

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

    std::cout << evaluate(represent) << std::endl;
    return 0;
}