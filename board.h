#ifndef BOARD_H
#define BOARD_H

#include <chrono>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "math.h"

// Special scrabble characters
#define EMPTY '-'
#define WILDCARD ' '
#define OUT_OF_BOUNDS '~'

// Width and height of the board
#define BOARD_SIZE 15

// Area for which the probability
#define PROB_CALC_SIZE 2
#define PROB_ARRAY_SIZE 5
constexpr int PROB_CALC_AREA = ((PROB_CALC_SIZE << 1) + 1) * ((PROB_CALC_SIZE << 1) + 1);
constexpr int PROB_CALC_AREA_DIRECTION = (PROB_CALC_SIZE + 1) << 2;

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
#define BONUS_TILES 1

/** 
 * Using a probablistic approach to finding a word on the board
 * Using a brute force method to find a word on the board
 */
#define PROBABILISTIC 0
#define BRUTE_FORCE 1 

// Define method for searching for a word
#define METHOD PROBABILISTIC

// File for the words in the scrabble dictionary
#define DICTIONARY "../scrabble_dictionary.txt"

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
    {'N', 2},      // N = 1 point
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
#define TRIPLE_LETTER 1
#define DOUBLE_LETTER 2
#define TRIPLE_WORD 3
#define DOUBLE_WORD 4

#if (BONUS_TILES)
    
    /**
     * The x-coordinate can be determined by finding modular of
     * the location with respect to 15 (location % 15)
     * 
     * The y-coordinate can be calculated by finding the integer
     * value of the quotient with respect to 15 ((int) location / 15)
     */

    static std::unordered_map<std::size_t, int> bonus_tile_locations = {
        /**
         * Triple letter locations on the board
         */
        {20, TRIPLE_LETTER},
        {24, TRIPLE_LETTER},
        {76, TRIPLE_LETTER},
        {80, TRIPLE_LETTER},
        {84, TRIPLE_LETTER},
        {88, TRIPLE_LETTER},
        {136, TRIPLE_LETTER},
        {140, TRIPLE_LETTER},
        {144, TRIPLE_LETTER},
        {148, TRIPLE_LETTER},
        {200, TRIPLE_LETTER},
        {204, TRIPLE_LETTER},

        /**
         * Double letter locatons on the board
         */
        {3, DOUBLE_LETTER},
        {11, DOUBLE_LETTER},
        {36, DOUBLE_LETTER},
        {38, DOUBLE_LETTER},
        {45, DOUBLE_LETTER},
        {52, DOUBLE_LETTER},
        {59, DOUBLE_LETTER},
        {92, DOUBLE_LETTER},
        {96, DOUBLE_LETTER},
        {98, DOUBLE_LETTER},
        {102, DOUBLE_LETTER},
        {108, DOUBLE_LETTER},
        {116, DOUBLE_LETTER},
        {122, DOUBLE_LETTER},
        {126, DOUBLE_LETTER},
        {128, DOUBLE_LETTER},
        {132, DOUBLE_LETTER},
        {165, DOUBLE_LETTER},
        {172, DOUBLE_LETTER},
        {179, DOUBLE_LETTER},
        {186, DOUBLE_LETTER},
        {188, DOUBLE_LETTER},
        {213, DOUBLE_LETTER},
        {221, DOUBLE_LETTER},

        /**
         * Triple word locations
         */
        {0, TRIPLE_WORD},
        {7, TRIPLE_WORD},
        {14, TRIPLE_WORD},
        {105, TRIPLE_WORD},
        {119, TRIPLE_WORD},
        {210, TRIPLE_WORD},
        {217, TRIPLE_WORD},
        {224, TRIPLE_WORD},

        /**
         * Double word locations
         */
        {16, DOUBLE_WORD},
        {28, DOUBLE_WORD},
        {32, DOUBLE_WORD},
        {42, DOUBLE_WORD},
        {48, DOUBLE_WORD},
        {56, DOUBLE_WORD},
        {64, DOUBLE_WORD},
        {70, DOUBLE_WORD},
        {154, DOUBLE_WORD},
        {160, DOUBLE_WORD},
        {168, DOUBLE_WORD},
        {176, DOUBLE_WORD},
        {182, DOUBLE_WORD},
        {192, DOUBLE_WORD},
        {196, DOUBLE_WORD},
        {208, DOUBLE_WORD},
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
    std::size_t points; // Point value of the letter on tile
    std::size_t x, y;   // x and y coordinates of the tile
    double probability; // Probability that a word can be made there

    /**
     * Tile default constructor
     * Defaults:
     *      Letter = EMPTY, Indicates that the tile is empty
     *      Points = 0, Indicates tile has no point value
     *      Probability = 0.0, Word can't be placed on an empty tile
     *      x, y = BOARD_SIZE, Tile has no coordinates until they are initialized 
     */
    Tile() : letter(EMPTY), points(0), probability(0.0), x(BOARD_SIZE), y(BOARD_SIZE) {};

    /**
     * Prints out the tile in the given format:
     * (x,y), letter, probability
     */
    void print() {
        std::cout << '(' << x << ',' << y << "), " << letter << ", " << probability << std::endl;
    }
} tile;

/**
 * Defines a potential move on the board.
 * The move consists of a sequence of letters (word),
 * a definitive amount of points depending on the placement 
 * of the word on the board, and an anchor point of the
 * move, which are the coordinates of the first letter.
 */
typedef struct Move {
    std::string word;              // Sequence of letters for move
    std::size_t points;                    // Points for word
    int anchorX, anchorY;          // Anchor point for the word
    int direction;                 // Direction for the move (down or right)
    std::size_t pivotX, pivotY;    // Tile for the move to pivot off of

    /**
     * Move default constructor
     * Defaults:
     *      Word = "", Indicates that there is no move
     *      Points = 0, Indicates a null move has no points
     *      anchorX, anchorY = BOARD_SIZE, Indicates the beginning of the word
     *      Direction = NO_DIRECTION, A null move has no direction
     */
    Move() : word(""), points(0), anchorX(BOARD_SIZE), anchorY(BOARD_SIZE), direction(NO_DIRECTION) {};

    /**
     * Move parameterized constructor
     */
    Move(std::string w, int p, int aX, int aY, int dir) : 
        word(w), points(p), anchorX(aX), anchorY(aY), direction(dir) {};

    /**
     * Prints out the move in the given format:
     * <word>; Location = (<x>, <y>); Direction = <direction>; Points for word = <points>
     */
    void print() {
        std::cout << "Word = " << word << "; Location = (" << anchorX << ", " << anchorY << "); ";
        if (direction == VERTICAL) std::cout << "DOWN; ";
        else std::cout << "RIGHT; ";
        std::cout << "Points for word = " << points << std::endl;
    }
} move;

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
 * Function prototypes
 **/

/**
 * Will be used to store all the words in the Scrabble dictionary
 * for the purpose of constant look-up time
 */
std::unordered_set<std::string> initializeWordSet(); // Method to initialize this set

Board createBoardFromFile(const std::string filename);

std::vector<std::string> getWordsOnBoard(const Board board);

int getEmptyNeighbors(const Board board, const Tile tile);

void printBoardValues(const Board board);

bool boardIsEmpty(const Board board);

std::vector<std::string> getPossibleWords(std::vector<char> letters, bool four_or_more);

Move findBestWord(Board& board, std::string letters);

std::size_t getPointValueOfWord(std::string word);

std::size_t getPointValueOfMove(Move& move);

#if (METHOD == PROBABILISTIC)

    int getBestDirection(const Board board, const Tile tile);

    void calcTileProbability(Board& board, Tile& tile);

    void getProbabilities(Board& board);

    Tile* getHighestProbabilities(Board& board);

    bool isPossibleMove(const Board board, std::unordered_set<std::string> scrabble_words, Move& move);

    template <typename T, std::size_t N>
    void insert(T (&arr)[N], T item, int idx) {
        for (int i = N - 1; i >= idx; --i) {
            // First iteration
            if (i == N - 1) { arr[i] = item; continue; }

            // Swap adjacent elements
            T temp1 = arr[i];
            T temp2 = arr[i + 1];
            arr[i] = temp2;
            arr[i + 1] = temp1;
        }
    }

#endif /* METHOD */

#endif /* BOARD_H */