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
    // printBoardValues(b_full);

    // std::cout << getEmptyNeighbors(b_full, b_full.tiles[4][3]) << std::endl;
    // std::cout << getEmptyNeighbors(b_full, b_full.tiles[4][8]) << std::endl;
    // std::cout << getEmptyNeighbors(b_full, b_full.tiles[10][4]) << std::endl;

    // getProbabilities(b_full);
    
    // std::cout << getBestDirection(b_full, b_full.tiles[10][1]) << std::endl;
    // std::cout << getBestDirection(b_full, b_full.tiles[7][10]) << std::endl;
    // std::cout << getBestDirection(b_full, b_full.tiles[5][3]) << std::endl;

    // Getting highest probability tiles
    // getHighestProbabilities(b_full);
    // printBoardValues(b_full);

    // Detemine if the letters make a word
    initializeWordSet();
    std::string test_string = "QUEEN";
    std::string gibberish = "ADFWERD";
    std::cout << scrabble_words.size() << std::endl;
    std::cout << scrabble_words.count(test_string) << std::endl;
    std::cout << scrabble_words.count(gibberish) << std::endl;
    

    /**
     * Testing certain methods
     */
    std::cout << "--------------------METHOD TESTING------------------------" << std::endl;
    char queen[5] = {'Q', 'U', 'E', 'E', 'N'};
    std::cout << "Point value for the word QUEEN is " << getPointValueOfWord(queen);

    return EXIT_SUCCESS;
}