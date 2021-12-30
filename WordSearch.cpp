#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define FILENAME "words_alpha.txt"

int main(int argc, char* argv[]) {
	std::vector<char> _letters, _compare;
	std::vector<std::string> _matches;
	std::string _input;
	bool _four_or_more = false;

	if ((argc == 3 && std::string(argv[1]) != "-f") || argc > 3) { 
		std::cout << "Incorrect number of inputs/Unknown Flag\n"; return EXIT_FAILURE; }
	else if (argc == 3) { _four_or_more = true; _input = argv[2]; }
	else { _input = argv[1]; }

	for (char c : _input) { _letters.push_back(c); }

	std::ifstream _file(FILENAME);
	if (_file.is_open()) {
		std::string _word, _temp;
		while (std::getline(_file, _word)) {
			_compare = _letters;
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
	
	for (std::string _word : _matches) {
		if (_four_or_more && _word.length() >= 4) std::cout << _word << '\n';
		else if (!_four_or_more && _word.length() >= 3) std::cout << _word << '\n'; 
	}
	return EXIT_SUCCESS;
}