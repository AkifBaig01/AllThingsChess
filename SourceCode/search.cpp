#include "moves.h"
#include "evaluate.h"

#include <stdio.h>
#include <vector>
#include <iostream>
#include <cstdint>

const int INF = 50000;
const int MATE = 49000;

int64_t nodes = 0;
int64_t ply = 0;

// Store best move (root)
Move best_move;

// negamax search with alpha-beta pruning 
int negamax(int alpha, int beta, int depth, full_pos& pos) {
    nodes++;

    std::vector<Move> legal_moves = legal_move_gen(pos);

    // Check terminal states first (important)
    int has_game_ended = game_ended(pos, legal_moves);
    if (has_game_ended != NONE) {
        switch (has_game_ended) {
            case STALEMATE:
            case INSUFMAT:
                return 0;
            case CHECKMATE:
                // Side to move is checkmated -> large negative score, prefer faster mates
                return -MATE + (int)ply;
            default:
                return 0;
        }
    }

    if (depth == 0) {
        return evaluate(pos);
    }

    for (Move &move : legal_moves) {
        // make move
        ply++;
        make_move(pos, move);

        int score = -negamax(-beta, -alpha, depth - 1, pos);

        // undo
        unmake_move(pos);
        ply--;

        // beta cutoff
        if (score >= beta) {
            return beta;
        }

        if (score > alpha) {
            alpha = score;
            // DO NOT set root best move here — we handle root in search()
            // If you did set here, make sure you only set when ply == 0:
            // if (ply == 0) best_move = move;
        }
    }

    return alpha;
}


// Search function (root loop handles best_move)
int search(int depth, full_pos& pos) {
    nodes = 0;
    ply = 0;
    best_move.from_sq = 0;
    best_move.to_sq   = 0;

    std::vector<Move> legal_moves = legal_move_gen(pos);
    if (legal_moves.empty()) {
        // handle no-move (mate/stalemate) as you prefer
        printf("bestmove (none)\n");
        return 0;
    }

    int alpha = -INF;
    int beta  =  INF;

    // root loop — explicitly set best_move here
    for (Move &move : legal_moves) {
        make_move(pos, move);
        ply++;
        int score = -negamax(-beta, -alpha, depth - 1, pos);
        ply--;
        unmake_move(pos);

        if (score > alpha) {
            alpha = score;
            best_move = move; // only set at root
        }

        if (alpha >= beta) {
            break; // root cutoff
        }
    }

    printf("info score cp %d depth %d nodes %lld\n", alpha, depth, (long long)nodes);
    printf("bestmove ");
    print_moves(best_move);
    printf("\n");
    return 0;
}
