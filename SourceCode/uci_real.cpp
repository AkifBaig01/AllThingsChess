#include "bitboard.h"
#include "uci.h"
#include "moves.h"
#include "search.h"
#include "utils.h"

#include <iostream>
#include <string>



// helper to map promotion letter to your Move.promo field
int promo_char_to_piece(char c) {
    switch (c) {
        case 'q': return QUEEN;
        case 'r': return ROOK;
        case 'b': return BISHOP;
        case 'n': return KNIGHT;
        default:  return 0;
    }
}

full_pos more_moves(full_pos& parsed, const std::vector<std::string>& moremoves) {
    for (const std::string &mvstr : moremoves) {
        if (mvstr.size() < 4) continue;
        int from = sqaure_to_index(mvstr.substr(0,2));
        int to   = sqaure_to_index(mvstr.substr(2,2));
        char promo_char = (mvstr.size() == 5 ? mvstr[4] : 0);
        int promo_piece = promo_char ? promo_char_to_piece(promo_char) : 0;

        // generate legal moves for the current position
        MoveList legal = legal_move_gen(parsed);

        bool applied = false;
        for (int i = 0; i < legal.count; i++) {
            Move m = legal.moves[i];
            if (m.from_sq == from && m.to_sq == to) {
                // if promotion present, ensure it matches
                if (promo_piece != 0) {
                    if (m.promotion == promo_piece) {
                        make_move(parsed, m);
                        applied = true;
                        break;
                    }
                } else {
                    // not a promotion, accept the move
                    make_move(parsed, m);
                    applied = true;
                    break;
                }
            }
        }

        if (!applied) {
            // couldn't find a matching legal move — fall back or report error
            std::cerr << "Warning: couldn't match move " << mvstr << " in legal move list\n";
            // Optionally attempt manual application (see fallback code)
        }
    }
    return parsed;
}


int uci_loop() {
    std::string command;
    full_pos represent;

    while(std::getline(std::cin, command)) {
        if (command == "uci") {
            std::cout << "id TheChessEngine" << std::endl;
            std::cout << "id author Akif Baig" << std::endl;
            std::cout << "uciok" << std::endl;
        }

        else if (command == "isready") {
            std::cout << "readyok" << std::endl;
        }

        else if (command == "quit") {
            break;
        }

        else if (command == "position startpos") {

            std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
            fen_rep board = fen_parser(fen); 
            represent = set_bitboard_pos(board);

        }

        else if (command.rfind("position fen ", 0) == 0) {

            std::string fen = command.substr(13); // grab everything after "position fen "
            fen_rep board = fen_parser(fen); 
            represent = set_bitboard_pos(board);

        }

        else if (command.rfind("go depth ", 0) == 0) {
            std::string no_depth = command.substr(9);
            int depth = std::stoi(no_depth);
            search(depth, represent);
            
        }

        // If none other conditions met means string is something like 
        // position startpos moves ... or position fen <fen string> moves
        // break it up using split and parse each piece

        std::vector<std::string> parts_of_command = split(command, ' ');

        // it is of type ---> position fen <fen string> moves ...
        if (parts_of_command[1] == "fen" && parts_of_command.size() > 2) {
            std::string fen = parts_of_command[2];
            fen_rep board = fen_parser(fen); 
            represent = set_bitboard_pos(board);

            // we have extra moves to parse
            std::vector<std::string> moves(parts_of_command.begin() + 2, parts_of_command.end());
            represent = more_moves(represent, moves);

        }

        else if (parts_of_command[1] == "startpos" && parts_of_command.size() > 2) {
            std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
            fen_rep board = fen_parser(fen); 
            represent = set_bitboard_pos(board);

            // we have extra moves to parse
            std::vector<std::string> moves(parts_of_command.begin() + 3, parts_of_command.end());
            represent = more_moves(represent, moves);
        }
    }

    return 0;
}
