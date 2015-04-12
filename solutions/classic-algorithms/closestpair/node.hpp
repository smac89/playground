#ifndef __KDNODE__
#define __KDNODE__

#include <array>
#include <ostream>
#include <type_traits>

namespace spatial {
	
	struct kdnode  {
		friend class kdtree;
		friend std::ostream& operator << (std::ostream&, const kdnode&);

		virtual ~kdnode();
		int getCardinal() const;

	protected:
		double getDim(int dim) const;
		virtual double distTo(kdnode &other) const = 0;

		// prevent instantiation by hiding this constructor
		template <std::size_t N>
		explicit kdnode(const std::array<double, N> &coords);

		double *coords;
		int cardinality;
		
	};

	struct kdnode2D : public kdnode {
		explicit kdnode2D(const std::array<double, 2> &coords);
		double distTo(kdnode &other) const;
	};

	struct kdnode3D : public kdnode {
		explicit kdnode3D(const std::array<double, 3> &coords);
		double distTo(kdnode &other) const;
	};
}

#endif // __KDNODE__