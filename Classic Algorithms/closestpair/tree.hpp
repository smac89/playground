#ifndef __KDTREE__
#define __KDTREE__

#include "node.hpp"
#include <vector>
#include <array>
#include <functional>
#include <random>
#include <iterator>
#include <algorithm>

namespace spatial {

    template <std::size_t N> class kdTree {

        double bestDist;
        
        /**
            Sorts the points given for the tree into an order that allows for easy searching
            @param start - the starting index in the points list
            @param end - the last position to search in the points list
            @param level - the current level of the tree
            @param stagger - used to keep the tree balanced
        */
        void sortPoints(int start, int end, int level, bool stagger=false) {
            if (start >= end) {
                return;
            }
    
            std::sort(&points[start], &points[end], [&](const kdNode<N>* n1, const kdNode<N>* n2) {
                return n1->getDim(level) < n2->getDim(level);
            });
    
            int mid = (start + end)/2;
            if ((start + end & 1) == 0) {
                if (stagger) {
                    mid--;
                }
                stagger = !stagger;
            }
    
            sortPoints(start, mid, (level + 1) % N, stagger);
            sortPoints(mid + 1, end, (level + 1) % N, stagger);
        }

        public:
            explicit kdTree(std::vector<kdNode<N> *> &list) {
                std::copy(list.begin(), list.end(), std::back_inserter(points));
                sortPoints(0, list.size(), 0);
            }

            /**
                @return a pair of points that are found to be the closest to each other
            */
            std::pair<kdNode<N>*, kdNode<N>*> closestPair() {
                double closest = std::numeric_limits<double>::max();
                std::pair<kdNode<N>*, kdNode<N>*> best = std::make_pair(nullptr, nullptr);
        
                for (kdNode<N>* node : points) {
                    kdNode<N> *close = this->nearestNeighbour(node);
                    if (bestDist < closest) {
                        best = std::make_pair(node, close);
                        closest = bestDist;
                    }
                }
                return best;
            }

        protected:
            std::vector<kdNode<N> *> points;

            /**
                @param start - the start of the range to search for a point in
                @param end - the end of the range to search for a point in
                @param level - the current level of the search space
                @param node - the node we are trying to find it's closest neighbour
            */
            kdNode<N>* nearestNeighbour(int start, int end, int level, kdNode<N>* node) {
                // start >= end means that we have nothing in the list to search for anymore
                if (start >= end) {
                    return nullptr;
                }

                int mid = (start + end) / 2;
                kdNode<N>* partition = points[mid], *close;

                // compare the nodes by the dimension corresponding to the current level
                // of the search space
                if (partition->getDim(level) > node->getDim(level)) { //search left side

                    // this is the closest node on the left hand side
                    close = nearestNeighbour(start, mid, (level + 1) % N, node);
                    start = mid + 1;    // update start incase we need to search other half
                }else {

                    // this is the closest neighbour in the right hand side
                    close = nearestNeighbour(mid + 1, end, (level + 1) % N, node);
                    end = mid;          // update end incase we need to search other half
                }

                // if the answer we got was not null and we didn't get the node we were searching for as an answer...
                if (close != nullptr && close != node) {

                    // get the distance from the closest point to the point in question
                    double temp = close->distTo(*node);

                    // distance from the closest point to any other point in the other half of
                    // the search space
                    double other = partition->distTo(*node);

                    // if we could find a better distance by search the other half...
                    if (temp > other) {

                        // search the other half
                        kdNode<N>* re = nearestNeighbour(start, end, (level + 1) % N, node);
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
                @param node - the node to find the nearest neighbour for
                @return - the closest point to the given kdNode
            */
            kdNode<N>* nearestNeighbour(kdNode<N>* node) {
                this->bestDist = std::numeric_limits<double>::max();
                return nearestNeighbour(0, points.size(), 0, node);
            }
    };


    template<std::size_t N>
    std::vector<kdNode<N>*> generateNodes(int num, double lo=1, double hi=10) {
        std::vector<kdNode<N>*> collection;

        static std::mt19937 mt(std::random_device{}());
        auto random = std::bind(std::uniform_real_distribution<double>(lo, hi), std::ref(mt));
        std::array<double, N> arr;

        std::generate_n(std::back_inserter(collection), num, [&]() {
            std::generate(arr.begin(), arr.end(), random);
            return new kdNode<N>(arr);
        });

        return collection;
    }

    // template <> std::vector<kdNode<2ul>*> generateNodes<2ul>(int, double, double);
    // template <> std::vector<kdNode<3ul>*> generateNodes<3ul>(int, double, double);
}
#endif // __KDTREE__