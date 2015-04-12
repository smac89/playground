#include "node.hpp"
#include "tree.hpp"
#include <iostream>
#include <vector>
#include <utility>
using namespace spatial;



int main() {
	std::vector<kdNode<2>*> nodes = generateNodes<2>(10);
	for (kdNode<2> *node : nodes) {
		std::cout << *node << std::endl;
	}

	kdTree<2> tree(nodes);
	std::pair<kdNode<2>*, kdNode<2>*> nearest = tree.closestPair();
	std::cout << std::endl << std::endl << *(nearest.first) << " " << *(nearest.second) << std::endl;
	return 0;
}