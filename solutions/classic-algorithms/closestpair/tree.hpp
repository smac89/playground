#ifndef __KDTREE__
#define __KDTREE__

#include "node.hpp"
#include <vector>

namespace spatial {

	class kdtree {
		std::vector<kdnode> points;

		public:
			explicit kdtree(std::vector<kdnode> &list);
	};
}
#endif // __KDTREE__