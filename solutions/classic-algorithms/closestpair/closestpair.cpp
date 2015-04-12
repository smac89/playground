#include "node.hpp"
#include "tree.hpp"
#include <iostream>
#include <vector>
using namespace spatial;

int main() {
	std::vector<kdnode2D*> nodes = kdtree::generateNodes<kdnode2D>(10);
	return 0;
}