#include "bitboard.h"
#include "uci.h"

#include <cstdint>
#include <iostream>

/* Very good idea to first create a helper function which takes in ULL ad returns a board 
representation */

/*
================================================
HELPER PRINT FOR DEBUGGING
================================================
*/

int main(){
    uint64_t test = fen_parsed();
    std::cout << "Current value " << test;
    return 0;
}







/*
=================================================
HELPER PRINT END
=================================================
*/



