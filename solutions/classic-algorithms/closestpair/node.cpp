#include "node.hpp"
#include <array>
#include <algorithm>
#include <stdexcept>
using namespace spatial;

int kdnode::getCardinal() const {
	return cardinality;
}

double kdnode::getDim(int dim) const {
	return coords[dim];
}

template <std::size_t N>
kdnode::kdnode(const std::array<double, N> &coords) {
	if (N <= 1) {
		throw std::invalid_argument("The cardinality has to be greater than 1");
	}
	this->cardinality = N;
	this->coords = new double[N];
	std::copy(coords.begin(), coords.end(), this->coords);
}

kdnode::~kdnode() {
	delete []coords;
}

kdnode2D::kdnode2D(const std::array<double, 2> &coords) 
: kdnode(coords) {}

kdnode3D::kdnode3D(const std::array<double, 3> &coords) 
: kdnode(coords) {}