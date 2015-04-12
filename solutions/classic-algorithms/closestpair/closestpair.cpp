#include "node.hpp"
#include "tree.hpp"
#include <iostream>
#include <vector>
#include <utility>
using namespace spatial;



int main() {
	std::vector<kdnode*> nodes = kdtree::generateNodes<kdnode2D>(10);
	for (kdnode *node : nodes) {
		std::cout << *node << std::endl;
	}

	kdtree tree(nodes);
	std::pair<kdnode*, kdnode*> nearest = tree.closestPair();
	std::cout << std::endl << std::endl << *(nearest.first) << " " << *(nearest.second) << std::endl;
	return 0;
}