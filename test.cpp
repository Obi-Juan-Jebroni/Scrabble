#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "board.h"

int main() {
    /**
     * Testing methods with an empty board
     */
    std::cout << "------------------------EMPTY-----------------------------" << std::endl;
    Board b_empty = createBoardFromFile("tests/empty.txt");

    /**
     * Testing methods with a large amount of words on the board
     */
    std::cout << "------------------------FULL------------------------------" << std::endl;
    Board b_full = createBoardFromFile("tests/test.txt");

    /**
     * Testing certain methods
     */
    std::cout << "--------------------METHOD TESTING------------------------" << std::endl;
    char queen[5] = {'Q', 'U', 'E', 'E', 'N'};
    std::cout << "Point value for the word QUEEN is " << getPointValueOfWord(queen);

    return EXIT_SUCCESS;
}