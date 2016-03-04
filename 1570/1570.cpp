#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>

using bridge = std::map<int, int>;

#ifdef DEBUG
std::ostream& operator <<(std::ostream& oss, const std::vector<int>& vec) {
	for (const int& item : vec) {
		oss << item << ' ';
	}
	return oss;
}

std::ostream& operator <<(std::ostream& oss, const bridge& map) {
	for (auto& item : map) {
		oss << '{' << item.first << " : " <<  item.second << "} ";
	}
	return oss;
}
#endif // DEBUG


int compute_result(std::vector<int>& reds, bridge& blues, int);

int main() {
	std::ios_base::sync_with_stdio(false);

	int tests, temp, n, r, b;
	std::map<int, int> blues;
	std::vector<int> reds;

	for (std::cin >> tests; tests--; ) {
		std::cin >> n >> r >> b;
		reds.clear();
		blues.clear();

		std::generate_n(std::back_inserter(reds), r, [&] {
			std::cin >> temp;
			return temp;
		});

		std::generate_n(std::inserter(blues, blues.begin()), b, [&] {
			std::cin >> temp;
			return std::make_pair(temp, -1);
		});

		#ifdef DEBUG
		std::cout << reds << '\n' << blues << '\n';
		#endif

		std::cout << compute_result(reds, blues, n) << '\n';
	}

	return 0;
}

int do_paring(int min_dist, std::vector<int>& reds, bridge& blues, int max_size) {

	int best = 0, temp;
	for (std::vector<int>::iterator trav = reds.begin(); trav != reds.end(); ) {
		int val = *trav;

		bridge::iterator iter = blues.find((val + min_dist) % max_size);
		if (iter != blues.end() && iter->second == -1) {
			iter->second = val;
			#ifdef DEBUG
			std::cout << blues << '\n';
			#endif
			trav = reds.erase(trav);
			best += min_dist;
			continue;
		}

		iter = blues.find((temp = (val - min_dist) % max_size) < 0 ? temp + max_size : temp);
		if (iter != blues.end() && iter->second == -1) {
			iter->second = val;
			#ifdef DEBUG
			std::cout << blues << '\n';
			#endif
			trav = reds.erase(trav);
			best += min_dist;
			continue;
		}

		trav++;
	}

	return best;
}

int compute_result(std::vector<int>& reds, bridge& blues, int max_size) {
	int ans = 0;
	for(int min_dist = 1; min_dist < max_size && !reds.empty(); min_dist++) {
		ans += do_paring(min_dist, reds, blues, max_size);
	}
	return ans;
}
