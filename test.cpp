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
    std::cout << "------------------------EMPTY-----------------------------\n\n";
    Board b_empty = createBoardFromFile("tests/empty.txt");
    Move best = findBestWord(b_empty, "QUENEST");
    best.print();

    /**
     * Testing methods with a large amount of words on the board
     */
    std::cout << "------------------------FULL------------------------------\n\n";
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
    // std::unordered_set<std::string> scrabble_words = initializeWordSet();
    // std::string test_string = "QUEEN";
    // std::string gibberish = "ADFWERD";
    // std::cout << scrabble_words.size() << std::endl;
    // std::cout << scrabble_words.count(test_string) << std::endl;
    // std::cout << scrabble_words.count(gibberish) << std::endl;

    // Determine if moves are valid
    // Move qi("QI", 12, 9, 3, VERTICAL);
    // std::cout << isPossibleMove(b_full, scrabble_words, qi) << std::endl;
    // Move ere("ERE", 0, 3, 7, VERTICAL);
    // std::cout << isPossibleMove(b_full, scrabble_words, ere) << std::endl;
    // Move eye("EYE", 0, 3, 7, VERTICAL);
    // std::cout << isPossibleMove(b_full, scrabble_words, eye) << std::endl;
    // Move alpha("ALPHA", 0, 1, 7, HORIZONTAL);
    // std::cout << isPossibleMove(b_full, scrabble_words, alpha) << std::endl;
    // Move alpha("ALPHA", 0, 1, 7, HORIZONTAL);
    // getPointValueOfMove(alpha);
    // std::cout << alpha.points << std::endl;
    Move best_move = findBestWord(b_full, "SPKMETA");
    best_move.print();


    /**
     * Testing certain methods
     */
    std::cout << "--------------------METHOD TESTING------------------------\n\n";
    // char queen[5] = {'Q', 'U', 'E', 'E', 'N'};
    // std::string like = "Like";
    // std::cout << like.find('e', 4) << ", " << std::string::npos << std::endl;

    return EXIT_SUCCESS;
}