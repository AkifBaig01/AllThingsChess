#include "uci.h"
#include <cstdint>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <bitset>

/*
Will be the function to send requests and recieve requests from the uci for now it will only be
concerned with the parsing of the fen string UCI tell and so on functions will be implemented later
as it should be realtively easy.
*/

std::string generate_random_board() {
    std::string test_board = "";
     for (int i = 0; i < 64; i++) {
        int random_num = rand() % 2;
        std::string temp = std::to_string(random_num);
        test_board += temp;
     }
    return test_board;
}

uint64_t fen_parsed() {
    std::string test_board = generate_random_board();
    std::cout << "String to convert " << test_board << "\n";

    std::string value = test_board;
    uint64_t a;
    char* end;
    a= strtoull( value.c_str(), &end, 2);
    std::bitset<64> bits_of_a(a);
    std::cout << "As uint64_t (binary):  " << bits_of_a.to_string() << "\n";
    // std::cout << "UInt64: " << a << "\n";

    return a;
}

int main() {
    fen_parsed();
    return 0;
}
