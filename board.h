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
#define OUT_OF_BOUNDS '~'

// Width and height of the board
#define BOARD_SIZE 15

// Area for which the probability 
#define PROB_CALC_SIZE 2
constexpr int PROB_CALC_AREA = (PROB_CALC_SIZE * 2 + 1) * (PROB_CALC_SIZE * 2 + 1);

// Maximum amount of neighbors a non-empty tile can have
#define MAX_NEIGHBORS 3

// Maximum amount of points a scrabble letter can have
#define MAX_LETTER_POINTS 10

/**
 * Direction that the word is placed
 * The word can either go vertically down
 * or the word can go horizontally right.
 */
#define VERTICAL 0
#define HORIZONTAL 1
#define NO_DIRECTION 2

/**
 * Determines whether or not bonus tiles are used or not
 * i.e: Triple letter, double letter, triple word, double word
 * 0 = No bonus tiles taken into account
 * 1 = Use bonus tiles to calculate best word
 */
#define BONUS_TILES 0

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
 * If the bonus tiles are being taken into account,
 * the locations for them are defined.
 */
#if(BONUS_TILES)
    
    /**
     * The x-coordinate can be determined by finding modular of
     * the location with respect to 15 (location % 15)
     * 
     * The y-coordinate can be calculated by finding the integer
     * value of the quotient with respect to 15 ((int) location / 15)
     */

    /**
     * Triple letter locations.
     */
    std::size_t _triple_letter_locations[12] = {
                                                20, 24, 76, 80,
                                                84, 88, 136, 140,
                                                144, 148, 200, 204
                                               };

    /**
     * Double letter locations
     */
    std::size_t _double_letter_locations[24] = {
                                                3, 11, 36, 38, 
                                                45, 52, 59, 92,
                                                96, 98, 102, 108,
                                                116, 122, 126, 128,
                                                132, 165, 172, 179,
                                                186, 188, 213, 221
                                               };

    /**
     * Triple word locations
     */
    std::size_t _triple_word_locations[8] = {
                                             0, 7, 14, 105,
                                             119, 210, 217, 224
                                            };

    /**
     * Double word locations
     */
    std::size_t _double_word_locations[16] = {
                                              16, 28, 32, 42,
                                              48, 56, 64, 70,
                                              154, 160, 168, 176,
                                              182, 192, 196, 208
                                             };
#endif



/**
 * A tile is the primary piece in the game of scrabble.
 * The tile contains one letter and a point value that
 * matches that letter according to the Scrabble rules.
 * The x and y coordinate are designated for the 
 */
typedef struct Tile {
    char letter;        // Letter on tile
    int points;         // Point value of the letter on tile
    std::size_t x, y;           // x and y coordinates of the tile
    double probability; // Probability that a word can be made there

    /**
     * Tile default constructor
     * Defaults:
     *      Letter = EMPTY, Indicates that the tile is empty
     *      Points = -1, Indicates tile has no point value
     *      Probability = 0.0, Word can't be placed on an empty tile
     *      x, y = BOARD_SIZE, Tile has no coordinates until they are initialized 
     */
    Tile() : letter(EMPTY), points(-1), probability(0.0), x(BOARD_SIZE), y(BOARD_SIZE) {};
} tile;

/**
 * A board is a double array consisting of tiles.
 * In reality, the board is 15x15.
 */
typedef struct Board {

    /**
     * Board initialized to a 15x15 tile array consisting of
     * empty tiles. The board can be created using the
     * "createBoardFromFile" method.
     */
    Tile tiles[BOARD_SIZE][BOARD_SIZE];

    /**
     * Returns the letter of the tile at the given coordinates
     * @param x
     *          X-coordinate
     * @param y
     *          Y-coordinate
     * @return Letter at the given coordinate
     *         EMPTY if the given coordinate is empty
     *         OUT_OF_BOUNDS if coordinates are outside of board
     */
    char getTile(std::size_t x, std::size_t y) const {
        if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
            return OUT_OF_BOUNDS;
        return tiles[x][y].letter;
    }
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
    for (std::size_t i = 0; i < N; ++i) _value += _letter_values[word[i]];
    return _value;
}

/**
 * Function prototypes
 **/

Board createBoardFromFile(const std::string filename);

std::vector<std::string> getWordsOnBoard(const Board board);

int getEmptyNeighbors(const Board board, const Tile tile);

void printBoardValues(const Board board);

bool boardIsEmpty(const Board board);

std::vector<std::string> getPossibleWords(std::vector<char> letters, bool four_or_more);

std::string findBestWord(Board& board);

#if (METHOD == PROBABILISTIC)

    // Array to store the highest probability locations
    static std::size_t highest_probability_locations[5];

    int getBestDirection(const Board board, const Tile tile);

    void calcTileProbability(Board& board, Tile& tile);

    void getProbabilities(Board& board);

    bool isPossibleMove(std::string word, std::size_t anchorX, std::size_t anchorY, bool direction);

#endif

#endif