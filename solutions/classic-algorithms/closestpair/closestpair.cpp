#include "node.hpp"
#include <iostream>

int main() {
	spatial::kdnode3D node({8.9, 9.9});
	std::cout << node << std::endl;
}