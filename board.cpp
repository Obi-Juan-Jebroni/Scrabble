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
 * Initializes the set to contain the words in the
 * Scrabble dictionary
 */
std::unordered_set<std::string> initializeWordSet() {
    std::unordered_set<std::string> new_set;
    std::ifstream _file(DICTIONARY);
    if (_file.is_open()) {
        std::string _word;
        while (std::getline(_file, _word))
            new_set.insert(_word);
    }
    else 
        std::cout << "Dictionary file location is wrong\n";
    
    return new_set;
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
				std::vector<char>::iterator it = find(_compare.begin(), _compare.end(), c);
				if (it != _compare.end()) { _compare.erase(it); _word.erase(find(_word.begin(), _word.end(), c)); }
				else break;
			}
			if (_word.empty()) { _matches.push_back(_temp); }
		}
	}
    else 
        std::cout << "Dictionary file location is wrong\n";

	return _matches;
}

/**
 * The probabilistic method is quicker but is potentially less
 * accurate than brute-forcing in an attempt to find the best word.
 */
#if (METHOD == PROBABILISTIC)

    /**
     * Finds the best direction for a word to be placed on
     * the target tile
     * @param board
     *          Scrabble board
     * @param tile
     *          Target tile on the scrabble board
     */
    int getBestDirection(const Board board, const Tile tile) {
        
        // If the tiles to the left and right of the target tile
        // are both empty, the best direction is horizontal
        if (board.getTile(tile.x - 1, tile.y) == EMPTY &&
            board.getTile(tile.x + 1, tile.y) == EMPTY) return HORIZONTAL;

        // If the tiles above and below the target tile
        // are both empty, the best direction is vertical
        if (board.getTile(tile.x, tile.y - 1) == EMPTY &&
            board.getTile(tile.x, tile.y + 1) == EMPTY) return VERTICAL;

        // If not, there is no "best direction"
        return NO_DIRECTION;
    }

    /**
     * Get probability of a tile with the given coordinates relative
     * to the current state of the board
     * @param board
     *          Scrabble board
     * @param tile
     *          Target tile on the scrabble board
     */
    void calcTileProbability(Board& board, Tile& tile) {
        double probability = 100;
        int neighbors = getEmptyNeighbors(board, tile);

        // If it has no neighbors, it's not possible to put a word
        // that orients off of that tile regardless of letters.
        if (!neighbors) return;

        // Get best direction if the number of neighbors
        // a target value has is or exceeds 2
        int best_direction = NO_DIRECTION;
        if (neighbors >= 2) 
            best_direction = getBestDirection(board, tile);

        /********************************************************************************************************
        *                                        Probability algorithm
        * 
        *********************************************************************************************************/
        
        int max_empty_proximity = 0, empty_proximity = 0;

        // If the best chance to get a word is vertically, search for vertical side neighbors
        if (best_direction == VERTICAL) {

            // Adjust probability based on neighbor status
            probability *= ((double) neighbors / (double) MAX_NEIGHBORS);

            // Used for calculating probabilites
            max_empty_proximity = PROB_CALC_AREA_DIRECTION;
            empty_proximity = max_empty_proximity;
            bool blocking_flag = false; // Used for when a non-empty tile is blocking the rest of the line

            for (int prox_y = (int) tile.y - PROB_CALC_SIZE + 1; prox_y <= (int) tile.y + PROB_CALC_SIZE + 1; ++prox_y) {

                // Don't count target tile
                if (prox_y == tile.y) continue;

                // Used for when future tiles are blocked by a non-empty tile in the same line
                if (blocking_flag) { 
                    empty_proximity -= 3;
                    continue;
                }

                // If the tile in the same line as the target tile is not empty
                if (board.getTile(tile.x, prox_y) != EMPTY) {
                    if (prox_y > tile.y) { blocking_flag = true; }
                    empty_proximity -= 3;
                    continue;
                }

                // Adjacent side tiles
                char left_letter = board.getTile(tile.x - 1, prox_y);
                char right_letter = board.getTile(tile.x + 1, prox_y);

                // Check if side tiles are empty
                if (left_letter != EMPTY && left_letter != OUT_OF_BOUNDS) empty_proximity--;
                if (right_letter != EMPTY && right_letter != OUT_OF_BOUNDS) empty_proximity--;
            }
        } // VERTICAL

        // If the best chance to get a word is vertically, search for vertical side neighbors
        else if (best_direction == HORIZONTAL) {

            // Adjust probability based on neighbors
            probability *= ((double) neighbors / (double) MAX_NEIGHBORS);

            // Used for calculating probabilities
            max_empty_proximity = PROB_CALC_AREA_DIRECTION;
            empty_proximity = max_empty_proximity;
            bool blocking_flag = false; // Used for when a non-empty tile is blocking the rest of the line

            for (int prox_x = (int) tile.x - PROB_CALC_SIZE; prox_x <= (int) tile.x + PROB_CALC_SIZE; ++prox_x) {

                // Don't count target tile
                if (prox_x == tile.x) continue;

                // Used for when future tiles are blocked by a non-empty tile in the same line
                if (blocking_flag) { 
                    empty_proximity -= 3;
                    continue;
                }

                // If the tile in the same line as the target tile is not empty
                if (board.getTile(prox_x, tile.y) != EMPTY) {
                    if (prox_x > tile.x) { blocking_flag = true; }
                    empty_proximity -= 3;
                    continue;
                }

                // Adjacent vertical tiles
                char top_letter = board.getTile(prox_x, tile.y - 1);
                char bottom_letter = board.getTile(prox_x, tile.y + 1);

                // Check if vertical tiles are empty
                if (top_letter != EMPTY && top_letter != OUT_OF_BOUNDS) empty_proximity--;
                if (bottom_letter != EMPTY && bottom_letter != OUT_OF_BOUNDS) empty_proximity--;
            }
        } // HORIZONTAL

        else {

            // Adjust probability based on neighbors
            // If the target tile doesn't have a direction, the probability is decreased drastically
            probability /= MAX_NEIGHBORS;

            // Used for calculating probabilities
            max_empty_proximity = PROB_CALC_AREA - 1;
            empty_proximity = max_empty_proximity;

            // Adjust probability based on amount of empty tiles around the target tile
            for (int prox_x = (int) tile.x - PROB_CALC_SIZE; prox_x <= (int) tile.x + PROB_CALC_SIZE; ++prox_x) {
                for (int prox_y = (int) tile.y - PROB_CALC_SIZE; prox_y <= (int) tile.y + PROB_CALC_SIZE; ++prox_y) {

                    // Don't count target tile
                    if (prox_x == tile.x && prox_y == tile.y) continue;

                    // If tile is not empty, subtract from potential empty tiles
                    if (board.getTile(prox_x, prox_y) != EMPTY) empty_proximity--;

                }
            }
        } // NO_DIRECTION

        // Adjusts probabilites based on empty tiles in proximity
        probability *= ((double) empty_proximity / (double) max_empty_proximity);

        // Adjust probability of finding a word based on target tile's letter
        probability -= 5 * ((double) tile.points / (double) MAX_LETTER_POINTS);

        // Changing the tile's probability to the new one
        tile.probability = probability;
    }

    /**
     * Retrieve probabilities for each tile on the board.
     * The probabilities represent how likely it would be
     * to place a word at that cross-section
     */
    void getProbabilities(Board& board) {

        // Iterates through entire board to calculate probability
        // that a word can be created on that tile.
        for (std::size_t y = 0; y < BOARD_SIZE; ++y) {
            for (std::size_t x = 0; x < BOARD_SIZE; ++x) {
                if (board.tiles[x][y].letter != EMPTY)
                    calcTileProbability(board, board.tiles[x][y]);
            }
        }
    }

    /**
     * Fills in the highest probabilities array 
     */
    Tile* getHighestProbabilities(Board& board) {

        // Initialize probabilities of tiles on the board
        getProbabilities(board);

        // Temporary array for storing highest probabilities
        double temp_prob[PROB_ARRAY_SIZE] = { 0.0 };
        
        // Array to store the highest probability locations
        static Tile highest_probability_tiles[PROB_ARRAY_SIZE];

        for (std::size_t y = 0; y < BOARD_SIZE; ++y) {
            for (std::size_t x = 0; x < BOARD_SIZE; ++x) {
                Tile& temp_tile = board.tiles[x][y];

                // Skip any empty tiles
                if (temp_tile.letter == EMPTY) continue;

                for (int idx = 0; idx < PROB_ARRAY_SIZE; ++idx) {

                    // If the current tile's prob is higher than the current index's prob
                    if (temp_tile.probability > temp_prob[idx]) {

                        // Insert probability in temp prob array
                        insert(temp_prob, temp_tile.probability, idx);

                        // Insert tile into highest prob array
                        insert(highest_probability_tiles, temp_tile, idx);

                        break;
                    }
                }
            }
        }
        return highest_probability_tiles;
    }

    /**
     * Determines if the move at the tile is possible or not
     * @param board
     *              Status of the Scrabble board
     * @param scrabble_words
     *              Set of Scrabble words
     * @param move
     *              Move to be evaluated for validity
     * @return True if the move is possible,
     *         False if the move violates the rules
     */
    bool isPossibleMove(const Board board, std::unordered_set<std::string> scrabble_words, Move& move) {
        // If the move direction is vertical, every tile that is placed vertically
        // must also make valid words with adjacent letters horizontally or must be isolated.
        if (move.direction == VERTICAL) {
            for (std::size_t i = 0; i < move.word.length(); ++i) {
                // Position of current tile relative to anchor point
                int y = move.anchorY + (int) i;
                
                // If one of the tiles is out of bounds, the move isn't possible
                if (board.getTile(move.anchorX, y) == OUT_OF_BOUNDS) return false;

                // If the tile is part of a word that has already been placed, ignore it
                if (move.pivotX == move.anchorX && move.pivotY == y)
                    continue;

                // Use deque to construct a word using push front and back
                std::deque<char> check_word;
                check_word.push_back(move.word[i]);
                
                // Check if there are any letters to the left
                int posX = move.anchorX;
                char prev_letter = board.getTile(--posX, y);
                while (prev_letter != EMPTY && prev_letter != OUT_OF_BOUNDS) {
                    check_word.push_front(prev_letter);
                    prev_letter = board.getTile(--posX, y);
                }

                // Check if there are any letters to the right
                posX = move.anchorX;
                char next_letter = board.getTile(++posX, y);
                while (next_letter != EMPTY && next_letter != OUT_OF_BOUNDS) {
                    check_word.push_back(next_letter);
                    next_letter = board.getTile(++posX, y);
                }

                // Convert deque sequence to string
                std::string word_from_deque;
                for (int i = 0; i < check_word.size(); ++i)
                    word_from_deque += check_word[i];

                // Check if the letter is isolated or if it makes a word with adjacent tiles
                if (word_from_deque.length() != 1 && !scrabble_words.count(word_from_deque)) {
                    return false;
                }

                // If it is a valid move, the move's points need to be adjusted for a new word being created
                if (word_from_deque.length() != 1)
                    move.points += getPointValueOfWord(word_from_deque);
            }

        } /* VERTICAL */

        // If the move direction is horizontal, every tile that is placed horizontally
        // must also make valid words with adjacent letters vertically or must be isolated.
        else {
            for (std::size_t i = 0; i < move.word.length(); ++i) {
                // Position of current tile relative to anchor point
                int x = move.anchorX + (int) i;

                // If one of the tiles is out of bounds, the move isn't possible
                if (board.getTile(x, move.anchorY) == OUT_OF_BOUNDS) return false;

                // If the tile is part of a word that is already played, ignore it
                if (move.pivotX == x && move.pivotY == move.anchorY)
                    continue;

                // Use deque to construct a word using push front and back
                std::deque<char> check_word;
                check_word.push_back(move.word[i]);
                
                // Check if there are any letters above
                int posY = move.anchorY;
                char prev_letter = board.getTile(x, --posY);
                while (prev_letter != EMPTY && prev_letter != OUT_OF_BOUNDS) {
                    check_word.push_front(prev_letter);
                    prev_letter = board.getTile(x, --posY);
                }

                // Check if there are any letters below
                posY = move.anchorY;
                char next_letter = board.getTile(x, ++posY);
                while (next_letter != EMPTY && next_letter != OUT_OF_BOUNDS) {
                    check_word.push_back(next_letter);
                    next_letter = board.getTile(x, ++posY);
                }

                // Convert deque sequence to string
                std::string word_from_deque;
                for (int i = 0; i < check_word.size(); ++i)
                    word_from_deque += check_word[i];

                // Check if the letter is isolated or if it makes a word with adjacent tiles
                if (word_from_deque.length() != 1 && !scrabble_words.count(word_from_deque)) {
                    return false;
                }

                // If it is a valid move, the move's points need to be adjusted for a new word being created
                move.points += getPointValueOfWord(word_from_deque);
            }

        } /* HORIZONTAL */

        return true;
    }

    /**
     * Finds the best word given the current state of the board
     * and a set of letters that the user has.
     * @param board
     *              State of the Scrabble board
     * @param letters
     *              Letters in the hands of the user
     * @return The best move to play given the board status and letters in hand
     */
    Move findBestWord(Board& board, std::string letters) {
        /**
         * If the board is empty, the first word must go through
         * the middle square on the Scrabble board (7, 7).
         * The first word can only be made of the tiles that
         * the user is currently holding in their hand.
         * The first word also automatically receives double points.
         */
        if (boardIsEmpty(board)) {
            
            // Initialize constants for the first move
            Move first_move;
            first_move.direction = HORIZONTAL;
            first_move.anchorY = (BOARD_SIZE >> 1);

            // Transforms string to vector of chars
            std::vector<char> letter_vector(letters.length());
            std::copy(letters.begin(), letters.end(), letter_vector.begin());

            // Retrieve all possible words
            std::vector<std::string> words_with_given_letters = getPossibleWords(letter_vector);

            // Temporary move is used for comparison with current best move
            Move temp_move;
            temp_move.direction = HORIZONTAL;
            temp_move.anchorY = (BOARD_SIZE >> 1);

            // Iterate through all possible words and find the one that gives the most points
            std::size_t points = 0;
            for (std::size_t idx = 0; idx < words_with_given_letters.size(); ++idx) {

                // Change properties of temporary word to find the points for that move
                std::string temp_word = words_with_given_letters[idx];
                temp_move.anchorX = (BOARD_SIZE >> 1) - (int) (temp_word.length() >> 1);
                temp_move.word = temp_word;
                std::size_t temp_points = getPointValueOfMove(temp_move);

                // If the temp move is better than prev move, make it the new best move
                if (temp_points > points) {
                    points = temp_points;
                    first_move.anchorX = temp_move.anchorX;
                    first_move.word = temp_word;
                }
            }

            // First move gets double points
            first_move.points = points << 1;
            return first_move;

        }
        
        /**
         * If the board is not empty, the algorithm will calculate the top locations
         * for finding a word (highest probabilities) and find the best words for 
         * those locations based on the letter on that tile and the tiles
         * that are currently in the player's hand.
         */
        Move best_move;
        std::unordered_set<std::string> scrabble_words = initializeWordSet();
        Tile* highest_probs = getHighestProbabilities(board);

        // Find the best move for each tile in the highest probabilities list
        for (std::size_t idx = 0; idx < PROB_ARRAY_SIZE; ++idx) {
            Move m;
            Tile target_tile = highest_probs[idx];
            m.pivotX = target_tile.x;
            m.pivotY = target_tile.y;
            std::string target_letters = letters + target_tile.letter;

            // Transforms string to vector of chars
            std::vector<char> letter_vector(target_letters.length());
            std::copy(target_letters.begin(), target_letters.end(), letter_vector.begin());

            m.direction = getBestDirection(board, target_tile);

            // Determine anchor points from direction
            if (m.direction == VERTICAL) m.anchorX = (int) target_tile.x;
            else if (m.direction == HORIZONTAL) m.anchorY = (int) target_tile.y;

            // Iterate through possible words on spot to find best word
            std::vector<std::string> possible_words = getPossibleWords(letter_vector);
            for (auto it = possible_words.begin(); it != possible_words.end(); ++it) {
                m.word = *it;
                
                // Determine anchor points from direction and placement of letter
                // on the current word being examined for validity
                std::size_t start = 0;
                std::size_t found_idx = m.word.find(target_tile.letter, start);
                while (found_idx != std::string::npos) {
                    
                    // Determine anchor points based on location of target tile and
                    // the target tile's letter's location in the word
                    if (m.direction == VERTICAL) m.anchorY = (int) target_tile.y - (int) found_idx;
                    else if (m.direction == HORIZONTAL) m.anchorX = (int) target_tile.x - (int) found_idx;

                    // Tests if the move is possible and then calculates the total points of the move
                    // auto timer_start = std::chrono::system_clock::now();
                    getPointValueOfMove(m);
                    if (m.points > best_move.points) {  
                        std::cout << m.points << ", " << best_move.points << std::endl;
                        if (isPossibleMove(board, scrabble_words, m)) {
                            m.print();
                            best_move.anchorX = m.anchorX;
                            best_move.anchorY = m.anchorY;
                            best_move.direction = m.direction;
                            best_move.points = m.points;
                            best_move.word = *it;
                        }
                    }
                    m.points = 0;
                    // auto timer_end = std::chrono::system_clock::now();
                    // std::chrono::duration<double> elapsed = timer_end - timer_start;
                    // std::cout << elapsed.count() << std::endl;

                    // Adjusts the start of searching for the target letter in the word
                    // in case there are duplicate letters of the target letter
                    start = found_idx + 1;
                    found_idx = m.word.find(target_tile.letter, start);
                }
            }

        }

        return best_move;
    }
#endif

/**
 * Using a brute-force method to find the best word
 */
#if (METHOD == BRUTE_FORCE)
    // TODO
    Move& findBestWord(Board board, std::string letters) {
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
                _new_tile.x = col;
                _new_tile.y = row;
                board.tiles[col++][row] = _new_tile;
            }
            ++row;
        }
    }
    else 
        std::cout << "File name not found\n";
    return board;
}

/**
 * Retrieves the point value of a word string
 * @param word
 *          Scrabble word
 * @return Point value of string parameter
 */
std::size_t getPointValueOfWord(std::string word) {
    std::size_t _value = 0;
    for (std::size_t i = 0; i < word.length(); ++i)
        _value += _letter_values[word[i]];
    return _value;
}

/**
 * Retrieves the scrabble point value of the given move on the board.
 * @param move
 *          Scrabble move played by user
 * @return Point value of the move on the board
 */
std::size_t getPointValueOfMove(Move& move) {
    std::size_t _value = 0;
    std::string _word = move.word;

    // If bonus tiles are active, use the coordinates of each tile
    // to determine if one of them is over a double or triple word.
    #if (BONUS_TILES)
        int double_flag = 0, triple_flag = 0;
        std::size_t single_coord = (move.anchorY * BOARD_SIZE) + move.anchorX;
        std::size_t pivot_coord = (move.pivotY * BOARD_SIZE) + move.pivotX;
    #endif

    for (std::size_t i = 0; i < _word.length(); ++i) {
        std::size_t letter_val = _letter_values[_word[i]];

        #if (BONUS_TILES)

            // If the tile was placed before the move, no special tiles will be applied
            if (single_coord == pivot_coord) {
                _value += letter_val;
                if (move.direction == VERTICAL) single_coord += 15;
                if (move.direction == HORIZONTAL) ++single_coord;
                continue;
            }
            // Searches map to see if the current tile location is on a bonus tile
            try {
                int special_move = bonus_tile_locations.at(single_coord);
                
                switch(special_move) {
                    case (TRIPLE_LETTER):
                        _value += letter_val << 1;
                        break;
                    case (DOUBLE_LETTER):
                        _value += letter_val;
                        break;
                    case (TRIPLE_WORD):
                        triple_flag++;
                        break;
                    case (DOUBLE_WORD):
                        double_flag++;
                        break;

                }
            }
            // Simply catches the exception so the program can run
            catch (std::out_of_range ex) {}

            _value += letter_val;

        #else
            _value += letter_val;
        #endif
        
        // Adjust single coordinate for next iteration based
        // on the direction of the move
        #if (BONUS_TILES)
            if (move.direction == VERTICAL) single_coord += 15;
            if (move.direction == HORIZONTAL) ++single_coord;
        #endif
    }

    // Checks if the double or triple word flags are up
    #if (BONUS_TILES)
        _value *= (std::size_t) pow(2, double_flag);
        _value *= (std::size_t) pow(3, triple_flag);
    #endif

    move.points += _value;
    return _value;
}

/**
 * Returns the neighbors of the given tile on the
 * respective board.
 * @param board
 *              Scrabble board
 * @param tile
 *              Tile on scrabble board
 * @return Number of neighbors the tile has relative
 *         to the scrabble board
 */
int getEmptyNeighbors(const Board board, const Tile tile) {
    int neighbors = 0;
    
    // Check if left neighbor empty
    char left = board.getTile(tile.x - 1, tile.y);
    if (left == EMPTY) neighbors++;

    // Check if right neighbor is empty
    char right = board.getTile(tile.x + 1, tile.y);
    if (right == EMPTY) neighbors++;

    // Check if upmost neighbor is empty
    char up = board.getTile(tile.x, tile.y - 1);
    if (up == EMPTY) neighbors++;

    // Check if downmost neighbor is empty
    char down = board.getTile(tile.x, tile.y + 1);
    if (down == EMPTY) neighbors++;

    return neighbors;
}

/**
 * Method for debugging
 * Prints board to console
 */ 
void printBoardValues(const Board board) {
    for (std::size_t y = 0; y < BOARD_SIZE; ++y) {
        for (std::size_t x = 0; x < BOARD_SIZE; ++x) {
            std::cout << board.tiles[x][y].probability << '\t';
        }
        std::cout << '\n';
    }
}