#include "tree.hpp"
#include "node.hpp"
#include <vector>
#include <iterator>
#include <limits>
#include <algorithm>

namespace spatial {

	kdtree::kdtree(std::vector<kdnode *> &list) {
		int cardinal = 0;
		for (kdnode* node : list) {
			if (cardinal == 0) cardinal = node->getCardinal();
			if (cardinal != node->getCardinal()) {
				throw std::invalid_argument("All nodes in the list must be of the same type");
			}
		}
		this->dim = cardinal;
		std::copy(list.begin(), list.end(), std::back_inserter(points));
	}

	void kdtree::sortPoints(int start, int end, int level, bool stagger) {
		if (start >= end) {
			return;
		}

		std::sort(&points[start], &points[end], [&](const kdnode* n1, const kdnode* n2) {
			return n1->getDim(level) < n2->getDim(level);
		});

		int mid = (start + end)/2;
		if ((start + end & 1) == 0) {
			if (stagger) {
				mid--;
			}
			stagger = !stagger;
		}

		sortPoints(start, mid, (level + 1) % dim, stagger);
		sortPoints(mid + 1, end, (level + 1) % dim, stagger);
	}


	kdnode* kdtree::nearestNeighbour(kdnode* node) {
		this->bestDist = std::numeric_limits<double>::max();
		return nearestNeighbour(0, points.size(), 0, node);
	}

	kdnode* kdtree::nearestNeighbour(int start, int end, int level, kdnode* node) {
		if (start >= end) {
			return nullptr;
		}

		int mid = (start + end) / 2;
		kdnode* partition = points[mid], *close;
		if (partition->getDim(level) > node->getDim(level)) { //search left side
			close = nearestNeighbour(start, mid, (level + 1) % dim, node);
			start = mid + 1;	// update start incase we need to search other half
		}else {
			close = nearestNeighbour(mid + 1, end, (level + 1) % dim, node);
			end = mid;			// update end incase we need to search other half
		}

		if (close != nullptr) {
			double temp = close->distTo(*node);
			double other = partition->distTo(*node);
			if (temp > other) {
				kdnode* re = nearestNeighbour(start, end, (level + 1) % dim, node);
				if (re != null)
					partition = re;
			}
			other = partition->distTo(*node);
			this->bestDist = std::min(other, temp);
			if (temp < other) {
				return close;
			}
		}

		return partition;
	}

	std::pair<kdnode*, kdnode*> kdtree::closestPair() {
		double closest = std::numeric_limits<double>::max();
		std::pair<kdnode*, kdnode*> best = std::make_pair(nullptr, nullptr);

		for (kdnode* node : points) {
			kdnode *close = this->nearestNeighbour(node);
			if (node->distTo(*close) < closest) {
				best = std::make_pair(node, close);
				closest = bestDist;
			}
		}
		return best;
	}
}