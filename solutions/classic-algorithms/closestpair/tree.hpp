#ifndef __KDTREE__
#define __KDTREE__

#include "node.hpp"
#include <vector>

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
	};
}
#endif // __KDTREE__