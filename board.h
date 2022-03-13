#ifndef BOARD_H
#define BOARD_H

#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#define EMPTY '-'
#define WILDCARD ' '

#define BOARD_SIZE 15

// Using a probablistic approach to finding a word on the board
#define PROBABILISTIC 0
// Using a brute force method to find a word on the board
#define BRUTE_FORCE 1 

// Define method for searching for a word
#define METHOD PROBABILISTIC

// File for the words in the scrabble dictionary
#define DICTIONARY "scrabble_dictionary.txt"

// Maps the letter to their scrabble point values
static std::unordered_map<char, int> _letter_values = {
    {EMPTY, -1},
    {WILDCARD, 0},
    {'A', 1},
    {'B', 3},
    {'C', 3},
    {'D', 2},
    {'E', 1},
    {'F', 4},
    {'G', 2},
    {'H', 4},
    {'I', 1},
    {'J', 8},
    {'K', 5},
    {'L', 1},
    {'M', 3},
    {'N', 1},
    {'O', 1},
    {'P', 3},
    {'Q', 10},
    {'R', 1},
    {'S', 1},
    {'T', 1},
    {'U', 1},
    {'V', 4},
    {'W', 4},
    {'X', 8},
    {'Y', 4},
    {'Z', 10},
};



/**
 * Struct types to construct scrabble board
 **/
typedef struct Tile {
    char letter = EMPTY;
    int points = -1;
    
    double probability = 0.0;
} tile;

typedef struct Board {
    Tile tiles[BOARD_SIZE][BOARD_SIZE];
} board;

/**
 * Retrieves the scrabble point value of the given word.
 * @param word
 *          Scrabble word
 * @return Point value of the string parameter
 */
template<std::size_t N>
std::size_t getPointValueOfWord(char (&word)[N]) {
    std::size_t _value = 0;
    for (std::size_t i = 0; i < N; ++i) {
        _value += _letter_values[word[i]];
    }
    return _value;
}

/**
 * Function prototypes
 **/

Board createBoardFromFile(const std::string filename);

std::vector<std::string> getWordsOnBoard(const Board board);

void printBoardValues(const Board board);

bool boardIsEmpty(const Board board);

std::vector<std::string> getPossibleWords(std::vector<char> letters, bool four_or_more);

#endif