#ifndef __KDTREE__
#define __KDTREE__

#include "node.hpp"
#include <vector>

namespace spatial {

	class kdtree {
		std::vector<kdnode *> points;
		int dim;

		void sortPoints(int start, int end, int level, bool stagger=false);

		public:
			explicit kdtree(std::vector<kdnode *> &list);
			std::pair<kdnode*, kdnode*> closestPair();
			kdnode* nearestNeighbour(kdnode* node);
	};
}
#endif // __KDTREE__