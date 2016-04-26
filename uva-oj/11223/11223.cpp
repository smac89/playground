#include <iostream>
#include <unordered_map>
#include <string>
#include <limits>
#include <sstream>

std::string decode_message(std::string);

int main() {
	int t;
	(std::cin >> t).ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::string line;

	for (int msg = 1; msg <= t; msg++) {
		std::getline(std::cin, line);
		std::cout << "Message #" << msg << '\n';
		std::cout << decode_message(line) << (msg < t ? "\n\n" : "\n");
	}
	return 0;
}

std::string decode_message(const std::string code) {

	static std::unordered_map<std::string, const char> symbol_table = {
		{".-", 'A'}, {".---", 'J'}, {"...", 'S'}, {".----", '1'}, {".-.-.-", '.'}, {"---...", ':'},
		{"-...", 'B'}, {"-.-", 'K'}, {"-", 'T'}, {"..---", '2'}, {"--..--", ','}, {"-.-.-.", ';'},
		{"-.-.", 'C'}, {".-..", 'L'}, {"..-", 'U'}, {"...--", '3'}, {"..--..", '?'}, {"-...-", '='},
		{"-..", 'D'}, {"--", 'M'}, {"...-", 'V'}, {"....-", '4'}, {".----.", '\''}, {".-.-.", '+'},
		{".", 'E'}, {"-.", 'N'}, {".--", 'W'}, {".....", '5'}, {"-.-.--", '!'}, {"-....-", '-'},
		{"..-.", 'F'}, {"---", 'O'}, {"-..-", 'X'}, {"-....", '6'}, {"-..-.", '/'}, {"..--.-", '_'},
		{"--.", 'G'}, {".--.", 'P'}, {"-.--", 'Y'}, {"--...", '7'}, {"-.--.", '('}, {".-..-.", '"'},
		{"....", 'H'}, {"--.-", 'Q'}, {"--..", 'Z'}, {"---..", '8'}, {"-.--.-", ')'}, {".--.-.", '@'},
		{"..", 'I'}, {".-.", 'R'}, {"-----", '0'}, {"----.", '9'}, {".-...", '&'}
	};

	std::string token, message;
	for (std::istringstream iss(code); iss >> token; ) {
		message += symbol_table[token];
		iss.get(); // Skip the first space
		if (iss.peek() == ' ') {
			message += ' ';
			iss.get();
		}
	}

	return message;
}
