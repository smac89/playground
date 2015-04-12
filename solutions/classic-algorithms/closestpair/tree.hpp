#ifndef __KDTREE__
#define __KDTREE__

#include "node.hpp"
#include <vector>
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

			// template magic to prevent creating kdnode types
			template<typename T>
			static typename std::enable_if<std::is_base_of<kdnode, T>::value &&
										  !std::is_same<kdnode, T>::value,
										  std::vector<T*>>::type generateNodes(int, double=1, double=10);
	};
}
#endif // __KDTREE__