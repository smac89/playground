#include "tree.hpp"
#include "node.hpp"
#include <vector>
#include <iterator>
#include <limits>
#include <algorithm>
#include <functional>
#include <random>

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

	/**
		Sorts the points given for the tree into an order that allows for easy seaching
		@param start - the starting index in the points list
		@param end - the last position to search in the points list
		@param level - the current level of the tree
		@param stagger - used to keep the tree balanced
	*/
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

	/**
		@param node - the node to find the nearest neighbour for
		@return - the closest point to the given kdnode
	*/
	kdnode* kdtree::nearestNeighbour(kdnode* node) {
		this->bestDist = std::numeric_limits<double>::max();
		return nearestNeighbour(0, points.size(), 0, node);
	}

	/**
		@param start - the start of the range to search for a point in
		@param end - the end of the range to search for a point in
		@param level - the current level of the search space
		@param node - the node we are trying to find it's closest neighbour
	*/
	kdnode* kdtree::nearestNeighbour(int start, int end, int level, kdnode* node) {
		// start >= end means that we have nothing in the list to search for anymore
		if (start >= end) {
			return nullptr;
		}

		int mid = (start + end) / 2;
		kdnode* partition = points[mid], *close;

		// compare the nodes by the dimension corresponding to the current level
		// of the search space
		if (partition->getDim(level) > node->getDim(level)) { //search left side

			// this is the closest node on the left hand side
			close = nearestNeighbour(start, mid, (level + 1) % dim, node);
			start = mid + 1;	// update start incase we need to search other half
		}else {

			// this is the closest neighbour in the right hand side
			close = nearestNeighbour(mid + 1, end, (level + 1) % dim, node);
			end = mid;			// update end incase we need to search other half
		}

		// if the answer we got was not null...
		if (close != nullptr) {

			// get the distance from the closest point to the point in question
			double temp = close->distTo(*node);

			// distance from the closest point to any other point in the other half of
			// the search space
			double other = partition->distTo(*node);

			// if we could find a better distance by search the other half...
			if (temp > other) {

				// search the other half
				kdnode* re = nearestNeighbour(start, end, (level + 1) % dim, node);
				if (re != nullptr)
					partition = re;
			}

			// calculate the distance again
			other = partition->distTo(*node);

			// set the best distance found
			this->bestDist = std::min(this->bestDist, std::min(other, temp));

			// if we did not succeed in finding a better distance, return the one we
			// initially found
			if (temp < other) {
				return close;
			}
		}

		// return the closest point found
		return partition;
	}

	/**
		@return a pair of points that are found to be the closest to each other
	*/
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

	template<typename T>
	typename std::enable_if<std::is_base_of<kdnode, T>::value &&
							!std::is_same<kdnode, T>::value,
							std::vector<T*>>::type kdtree::generateNodes(int num, double lo, double hi) {
		std::vector<T*> collection;

		static std::random_device rd;
    	static std::mt19937 mt(rd());
    	auto dist = std::bind(std::uniform_real_distribution<double>(lo, hi), mt);

    	// std::generate_n(std::back_inserter(collection), num, [&]() {

    	// });

		return collection;
	}
}