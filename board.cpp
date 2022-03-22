#include "board.h"

/**
 * Checks if the board is empty or not
 * @param board
 *         Scrabble board
 * @return True if the board is empty, else return false
 */
bool boardIsEmpty(const Board board) {
    // A word will always pass through the middle tile
    return (board.tiles[7][7].letter == '-');
}

/**
 * Retrieves all possible words given the combination of letters
 * @param letters
 *          Find all possible words using these letters
 * @return Vector full of possible words given the letters
 */
std::vector<std::string> getPossibleWords(std::vector<char> letters) {
	std::vector<char> _compare;
	std::vector<std::string> _matches, _adjusted_matches;

	std::ifstream _file(DICTIONARY);
	if (_file.is_open()) {
		std::string _word, _temp;
		while (std::getline(_file, _word)) {
			_compare = letters;
			_temp = _word;
			while (!_word.empty()) {
				char c = _word[0];
				auto it = find(_compare.begin(), _compare.end(), c);
				if (it != _compare.end()) { _compare.erase(it); _word.erase(find(_word.begin(), _word.end(), c)); }
				else break;
			}
			if (_word.empty()) { _matches.push_back(_temp); }
		}
	}
	return _matches;
}

/**
 * The probabilistic method is quicker but is potentially less
 * accurate than brute-forcing in an attempt to find the best word.
 */
#if (METHOD == PROBABILISTIC)
    static std::tuple<std::size_t, std::size_t> highest_probability_coords[5];

    /**
     * Get probability of a tile with the given coordinates relative
     * to the current state of the board
     */
    void calcTileProbability(Board& board, Tile& tile,
                              const std::size_t x, const std::size_t y) {
        
    }

    /**
     * Retrieve probabilities for each tile on the board.
     * The probabilities represent how likely it would be
     * to place a word at that cross-section
     */
    void getProbabilities(Board& board) {
        for (std::size_t y = 0; y < BOARD_SIZE; ++y) {
            for (std::size_t x = 0; x < BOARD_SIZE; ++x) {
                Tile curr_tile = board.tiles[x][y];
                if (curr_tile.letter != EMPTY)
                    calcTileProbability(board, curr_tile, x, y);
            }
        }
    }

    // TODO: Implement method
    std::string findBestWord(Board& board) {
        if (boardIsEmpty(board)) {

        }
        return "";
    }
#endif

/**
 * Using a brute-force method to find the best word
 */
#if (METHOD == BRUTE_FORCE)
    // TODO
    std::string findBestWord(Board board) {
        return "";
    }
#endif

/**
 * Creates the scrabble board from the given file
 * @param filename 
 *         Name of file to make a scrabble board out of
 * @return Board with the given letters
 */
Board createBoardFromFile(const std::string filename) {
    Board board;

    std::ifstream _file(filename);
	if (_file.is_open()) {
		std::string _line;
        std::size_t row = 0;
		while (std::getline(_file, _line)) {
            std::size_t col = 0;
            for (char c : _line) {
                c = std::toupper(c);
                Tile _new_tile;
                _new_tile.letter = c;
                _new_tile.points = _letter_values[c];
                board.tiles[col][row] = _new_tile;
                col++;
            }
            row++;
        }
    }
    else 
        std::cout << "File name not found\n";
    return board;
}

// std::vector<std::string> getWordsOnBoard(const Board board) {
//     std::vector<std::string> _words;
    
//     return _words;
// }

/**
 * Method for debugging
 * Prints board to console
 */ 
void printBoardValues(const Board board) {
    for (std::size_t y = 0; y < BOARD_SIZE; ++y) {
        for (std::size_t x = 0; x < BOARD_SIZE; ++x) {
            std::cout << board.tiles[x][y].points << ' ';
        }
        std::cout << '\n';
    }
}