#ifndef __KDNODE__
#define __KDNODE__

#include <initializer_list>
#include <array>

namespace spatial {
	
	struct kdnode {
		virtual ~kdnode();
		int getCardinal() const;

	protected:
		template <std::size_t N>
		explicit kdnode(const std::array<double, N> &coords);
		int cardinality;
		double *coords;

	};

	struct kdnode2D : public kdnode {
		explicit kdnode2D(const std::array<double, 2> &coords);
	};

	struct kdnode3D : public kdnode {
		explicit kdnode3D(const std::array<double, 3> &coords);
	};
}

#endif // __KDNODE__