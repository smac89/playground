#include <iostream>
#include <regex>
#include <string>
#include <limits>
#include <functional>

int main() {

	std::function<std::string()> read_next = [&] {
		static std::string line;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::getline(std::cin, line);
		return line;
	};

	try {
		std::regex pattern("(.+?)\\1+", std::regex::optimize);
		std::smatch match;
		int t;
		(std::cin >> t).ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		for (int v = 1; v <= t; v++) {
			std::string line = read_next();
			if (std::regex_match(line, match, pattern)) {
				std::cout << match[1].length() << '\n';
			} else {
				std::cout << line.length() << '\n';
			}
			if (v != t) {
				std::cout << '\n';
			}
		}
	} catch (const std::regex_error& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	return 0;
}
