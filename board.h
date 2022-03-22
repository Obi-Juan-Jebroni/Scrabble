#ifndef BOARD_H
#define BOARD_H

#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

// Special scrabble characters
#define EMPTY '-'
#define WILDCARD ' '

// Width and height of the board
#define BOARD_SIZE 15

// Area for which the probability 
#define PROB_CALC_SIZE 3

/** 
 * Using a probablistic approach to finding a word on the board
 * Using a brute force method to find a word on the board
 */
#define PROBABILISTIC 0
#define BRUTE_FORCE 1 

// Define method for searching for a word
#define METHOD PROBABILISTIC

// File for the words in the scrabble dictionary
#define DICTIONARY "scrabble_dictionary.txt"

/**
 * Maps the letters to their scrabble point values
 */
static std::unordered_map<char, int> _letter_values = {
    {EMPTY, -1},   // An empty space is simply a negative value for consistency
    {WILDCARD, 0}, // A wildcard can be any letter
    {'A', 1},      // A = 1 point
    {'B', 3},      // B = 3 points
    {'C', 3},      // C = 3 points
    {'D', 2},      // D = 2 points
    {'E', 1},      // E = 1 point
    {'F', 4},      // F = 4 points
    {'G', 2},      // G = 2 points
    {'H', 4},      // H = 4 points
    {'I', 1},      // I = 1 point
    {'J', 8},      // J = 8 points
    {'K', 5},      // K = 5 points
    {'L', 1},      // L = 1 point
    {'M', 3},      // M = 3 points
    {'N', 1},      // N = 1 point
    {'O', 1},      // O = 1 point
    {'P', 3},      // P = 3 points
    {'Q', 10},     // Q = 10 points
    {'R', 1},      // R = 1 point
    {'S', 1},      // S = 1 point
    {'T', 1},      // T = 1 point
    {'U', 1},      // U = 1 point
    {'V', 4},      // V = 4 points
    {'W', 4},      // W = 4 points
    {'X', 8},      // X = 8 points
    {'Y', 4},      // Y = 4 points
    {'Z', 10},     // Z = 10 points
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