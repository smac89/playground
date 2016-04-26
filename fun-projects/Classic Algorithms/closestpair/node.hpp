#ifndef __KDNODE__
#define __KDNODE__

#include <array>
#include <cmath>
#include <ostream>
#include <numeric>
#include <algorithm>
#include <type_traits>

namespace spatial {
    template <std::size_t N> class kdTree;

    template <std::size_t N>
    struct kdNode  {

        explicit kdNode(const std::array<double, N> &coords) {
            if (N <= 1) {
                throw std::invalid_argument("The cardinality has to be greater than 1");
            }
            this->coords = new double[N];
            std::copy(coords.begin(), coords.end(), this->coords);
        }

        template <std::size_t T>
        friend std::ostream& operator << (std::ostream& oss, const kdNode<T>& node) {
            oss << "{" << node.coords[0];
            for (int t = 1; t < T; t++) {
                oss << ", " << node.coords[t];
            }
            oss << "}";
            return oss;
        }

        virtual ~kdNode() {
            delete []coords;
        }


    protected:
        double getDim(int dim) const {
            return coords[dim];
        }

        double distTo(kdNode<N> &other) const {
            int index = 0;
            double sump = std::accumulate(coords, &coords[N], 0,
                [&](double accum, double next) {
                    return accum + std::pow((next - other.coords[index++]), 2);
                }
            );
            return std::sqrt(sump);
        }

        double *coords;
        friend class kdTree<N>;
    };
}

#endif // __KDNODE__
