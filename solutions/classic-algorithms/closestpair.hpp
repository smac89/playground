#ifndef __CLOSESTPAIR__
#define __CLOSESTPAIR__

#include <vector>
#include "node.hpp"

namespace spatial {

	class kdtree {
		std::vector<kdnode> points;

		public:
			explicit kdtree(std::vector<kdnode> &list);
	};
}
#endif // __CLOSESTPAIR__