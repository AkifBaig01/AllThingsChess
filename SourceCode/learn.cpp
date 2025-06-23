#include <vector>
#include <iostream>
#include <string>


/*
Task: Declare a static array of 10 integers, initialize it to the values 1 through 10, then 
write a for-loop (index-based) that computes and prints the sum of its elements.
*/

int main() {
    std::vector<int> arr(10);

    for (int i = 0; i < 10; i++) {
        arr[i] = i + 1;
    }

    for (int i = 0; i < 10; i++) {
        std::cout << arr[i] << " ";
    }

    int total = 0;

    for (int i = 0; i < 10; i++)  {
        total += arr[i];
    }

    std::cout << "\n";
    std::cout << "the total is " << total;
    return 0;

}