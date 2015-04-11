#include "tree.hpp"
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
		this->dim = cardinal;
		std::copy(list.begin(), list.end(), std::back_inserter(points));
	}

	void kdtree::sortPoints(int start, int end, int level, bool stagger=false) {
		if (start >= end) {
			return;
		}

		std::sort(&points[start], &points[end], [&](const kdnode& n1, const kdnode& n2) {
			return n1.getDim(level) < n1.getDim(level);
		});

		int mid = (start + end)/2;
		if ((start + end & 1) == 0) {
			if (stagger) {
				mid++;
			}
			stagger = !stagger;
		}

		sortPoints(start, mid, (level + 1) % dim, stagger);
		sortPoints(mid + 1, end, (level + 1) % dim, stagger);
	}

	std::pair<kdnode, kdnode> kdtree::closestPair() {

	}
}