#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "board.h"

int main(int argc, char* argv[]) {
	std::string _input;
	std::vector<char> _letters;
	bool _four_or_more = false;

	if ((argc == 3 && std::string(argv[1]) != "-f") || argc > 3) { 
		std::cout << "Incorrect number of inputs/Unknown Flag\n"; return EXIT_FAILURE; }
	else if (argc == 3) { _four_or_more = true; _input = argv[2]; }
	else { _input = argv[1]; }
	for (char c : _input) { _letters.push_back(c); }

	std::vector<std::string> _possible_words = getPossibleWords(_letters, _four_or_more);

	Board b = createBoardFromFile("tests/test.txt");

	return EXIT_SUCCESS;
}