#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>

auto main() -> int {

	std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);

	std::function<bool(std::string::iterator, std::string::iterator)> 
	is_palindrome = [](std::string::iterator fst, 
		std::string::iterator lst) -> bool {
		while (fst <= lst) {
			if (*fst++ != *lst--) {
				return false;
			}
		}
		return true;
	};

	std::for_each(std::istream_iterator<std::string>(std::cin),
		std::istream_iterator<std::string>(),
		[&](std::string str) {
			std::vector<std::string> palindromes;
			int len = str.length();
			palindromes.reserve(len * (len - 1) / 2 + 1);

			/**
			 * Brute force algorithm generates all substrings and
			 * checks if each one is a palindrome before placing it
			 * in the vector
			 */
			for (int l = 1; l < len; ++l) {
				for (int s = 0, e = s + l; e < len; ++s, ++e) {
					if (is_palindrome(str.begin() + s, str.begin() + e)) {
						palindromes.emplace_back(str.substr(s, e - s + 1));
					}
				}
			}
			
			std::sort(palindromes.begin(), palindromes.end());
			auto last = std::unique(palindromes.begin(), palindromes.end());

			std::copy(palindromes.begin(), last,
				std::ostream_iterator<std::string>(std::cout, "\n"));
			std::cout << "\n";
		});

	return 0;
}
