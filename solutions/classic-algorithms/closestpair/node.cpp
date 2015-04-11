#include "node.hpp"
#include <array>
#include <algorithm>
#include <stdexcept>
#include <typeinfo>
#include <cmath>
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

double kdnode2D::distTo(kdnode& other) const {
	try {
		kdnode2D& temp = dynamic_cast<kdnode2D&>(other);
		double diffx = coords[0] - temp.coords[0];
		double diffy = coords[1] - temp.coords[1];
		return std::sqrt(std::pow(diffx, 2) + std::pow(diffy, 2));
	} catch (const std::bad_cast& e) {
		throw std::invalid_argument("Cannot get distTo for nodes of different cardinality!");
	}
}

kdnode3D::kdnode3D(const std::array<double, 3> &coords) 
: kdnode(coords) {}

double kdnode3D::distTo(kdnode& other) const {
	try {
		kdnode3D& temp = dynamic_cast<kdnode3D&>(other);
		double diffx = coords[0] - temp.coords[0];
		double diffy = coords[1] - temp.coords[1];
		double diffz = coords[2] - temp.coords[2];
		return std::sqrt(std::pow(diffx, 2) + std::pow(diffy, 2) + std::pow(diffz, 2));
	} catch (const std::bad_cast& e) {
		throw std::invalid_argument("Cannot get distTo for nodes of different cardinality!");
	}
}