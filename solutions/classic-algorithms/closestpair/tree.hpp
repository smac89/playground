#ifndef __KDTREE__
#define __KDTREE__

#include "node.hpp"
#include <vector>
#include <functional>
#include <random>
#include <array>
#include <algorithm>
#include <type_traits>

namespace spatial {

	class kdtree {
		void sortPoints(int start, int end, int level, bool stagger=false);
		double bestDist;

		protected:
			std::vector<kdnode *> points;
			int dim;
			kdnode* nearestNeighbour(int start, int end, int level, kdnode* node);
			kdnode* nearestNeighbour(kdnode* node);

		public:
			explicit kdtree(std::vector<kdnode *> &list);

			std::pair<kdnode*, kdnode*> closestPair();

			template<typename T>
			static typename std::enable_if<std::is_base_of<kdnode, T>::value &&
											!std::is_same<kdnode, T>::value,
											std::vector<kdnode*>>::type generateNodes(int num, double lo=1, double hi=10) {
				std::vector<kdnode*> collection;

    			static std::mt19937 mt(std::random_device{}());
    			auto random = std::bind(std::uniform_real_distribution<double>(lo, hi), std::ref(mt));

    			std::generate_n(std::back_inserter(collection), num, [&]() {
    				T *temp = new T({});
    				std::generate(temp->coords, &temp->coords[temp->getCardinal()], std::ref(random));
    				return temp;
    			});

				return collection;
			}
	};
}
#endif // __KDTREE__