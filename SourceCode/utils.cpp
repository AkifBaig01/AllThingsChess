#include "utils.h"

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

// Function splits up a string using a given delim
std::vector<std::string> split(const std::string& lines, char delim){

    // Intialise vector object each elt stored as string 
    std::vector<std::string> items;

    // Convert string to stream object to get accesss to processing tools 
    std::istringstream school(lines);

    // Intialise string variable 
    std::string token;

    // Goes through the given stream object returns true if char availible untill
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

    std::cout << rank * 8 + file << '\n';

    // standard formula for output index
    return rank * 8 + file;
}


// ALL DECLARATIONS