#include "closestpair.hpp"
#include "node.hpp"
#include <vector>
#include <iterator>
#include <algorithm>

namespace spatial {
	kdtree::kdtree(std::vector<kdnode> &list) {
		int cardinal = 0;
		for (kdnode& node : list) {
			if (cardinal == 0) cardinal = node.getCardinal();
			if (cardinal != node.getCardinal()) {
				throw std::invalid_argument("All nodes in the list must be of the same type");
			}
		}
		std::copy(list.begin(), list.end(), std::back_inserter(points));
	}

	void sortTree(kdtree &tree) {

	}
}

int main() {
	spatial::kdnode2D node({8.9, 9.9});
}