#include "utils.h"

#include <cstdint>
#include <string>
#include <sstream>
#include <vector>



// ALL DECLARATIONS

// Function splits up a string using a given delim
std::vector<std::string> split(const std::string& lines, char delim){

    // Intialise vector object, each elt stored as string 
    std::vector<std::string> items;

    // Convert string to stream object to get accesss to processing tools
    // school used to show name is not important 
    std::istringstream school(lines);

    // Intialise string variable 
    std::string token;

    // Goes through the given stream object returns true if char availible until
    // Next delim or false if not stores chars in token and returns that
    // get line requires stream object not stirng 
    while (std::getline(school, token, delim)) {

        // Add to dynamaic array 
        items.push_back(token);
    }

    // Return dynamic array
    return items;
}


// takes a given sqaure like "e4" and parses it to an index
int sqaure_to_index(const std::string& square) {
    // Ordering of file and rnak is consistent for board indexed at a8 = 0
    int file = square[0] - 'a';
    int rank = '8' - square[1];

    // standard formula for output index
    return rank * 8 + file;
}

void print_bitboard(uint64_t bitboard) {
    // Loop over ranks
    for (int rank = 0; rank <8; rank++) {
        // Loop over board files
        for (int file = 0; file < 8; file++) {
            // convert file and rank into square index
            int square= rank * 8 + file;
            
            // print ranks 
            if (!file)
                printf("%d - ", 8 - rank);

            // Left shift 1 by the square index do a bitwise and operation return 1 if ture else 0
            printf(" %d ", get_bit(bitboard, square) ? 1 : 0);
        }

        // Print new line every rank 
        printf("\n");
    }

    // print board files
    printf("\n     a  b  c  d  e  f  g  h\n\n");

    // print bitbaord as an unsigned decimal number 
    printf("Bitboard: %llud\n\n", bitboard);
}


std::string index_to_square(int index) {
    const std::string files = "abcdefgh";
    int file = index % 8;         // 0 = a, 1 = b, ..., 7 = h
    int rank = 8 - (index / 8);   // 0 = rank 8, ..., 7 = rank 1

    return std::string{files[file]} + std::to_string(rank);
}

// ALL DECLARATIONS