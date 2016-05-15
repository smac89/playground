#include <iostream>
#include <array>
#include <utility>
#include <tuple>
#include <algorithm>

constexpr std::size_t nodes_count = 10 * 10 * 100;

struct node {
    enum struct gate : int { AND=1, OR, NONE };
    bool value, changeable;
    gate g;
};

std::ostream& operator << (std::ostream& oss, const node& v) {
    oss << std::boolalpha << "{value=" << v.value <<
    ", changeable=" << v.changeable <<
    ", gate=";
    switch(v.g) {
        case node::gate::AND:
            return oss << "AND}";
        case node::gate::OR:
            return oss << "OR}";
        default:
            return oss << "NONE}";
    }
}

template <class T>
void logMessage(const T& message, std::ostream& out = std::cerr) {
    #ifdef DEBUG_MODE
    out << message << '\n';
    #endif
}

template <class Iter>
void logRange(Iter begin, Iter end, std::ostream& out = std::cerr) {
    #ifdef DEBUG_MODE
    while (begin != end) {
        logMessage(*begin++, out);
    }
    #endif
}

int post_order_fix(node* tree, int pos, const bool goal);

inline static constexpr int parent(int pos) {
    return pos == 0 ? -1 : (pos - 1) >> 1;
}

int main(int argc, char const *argv[]) {

    // Remove synchronization between cstdio and iostream
    std::ios_base::sync_with_stdio(false);
    // Prevent flushing of output stream before each io operation
    std::cin.tie(nullptr);

    std::array<node, nodes_count> tree;

    int N, M, V, s;
    std::cin >> N;
    for (int c = 1; c <= N; c++) {
        std::cin >> M >> V;

        // Since M will be odd, no need to subtract 1 before dividing
        int interior_nodes = M >> 1;

        std::for_each(tree.begin(), tree.begin() + interior_nodes, [](node& v) {
            std::cin >> v.value;
            v.g = v.value ? node::gate::AND : node::gate::OR;
            std::cin >> v.changeable;
        });

        std::for_each(tree.begin() + interior_nodes, tree.begin() + M, [](node& v) {
            std::cin >> v.value;
            v.g = node::gate::NONE;
        });

        for (int v = M - 1; v > 0; v -= 2) {
            int p = parent(v);
            tree[p].value = tree[p].g == node::gate::AND ?
            tree[v].value && tree[v - 1].value : tree[v].value || tree[v - 1].value;
        }

        logRange(tree.begin(), tree.begin() + M);

        s = post_order_fix(tree.data(), 0, V);

        std::cout << "Case #" << c << ": ";
        if (s != -1) {
            std::cout << s << '\n';
        } else {
            std::cout << "IMPOSSIBLE\n";
        }
    }
    return 0;
}

inline static constexpr std::pair<int, int> left_right(int pos) {
    return {(pos << 1) + 1, (pos << 1) + 2};
}

inline static constexpr bool is_left_sib(int pos) {
    return pos != 0 && (pos & 1);
}

inline static constexpr bool is_right_sib(int pos) {
    return pos != 0 && ((pos & 1) == 0);
}

/**
 *    Fixes the boolean tree to make the value at the root match the goal.
 *    @param  tree A pointer to the array containing the tree values
 *    @param  pos  The current node to consider
 *    @param  goal The value expected for the root node
 *    @return   a non negative value if it is possible for the root node to
 *    have the value specified by goal, otherwise -1 to indicate it is impossible
 *    The algorithm is as follows:
 *    If value != goal
 *        if not changeable
 *            if "AND"
 *                if goal, want both children to be 1
 *                if not goal, want one child to be 0
 *            if "OR"
 *                if goal, want one child to be 1
 *                if not goal, want both to be zero
 *        if changeable
 *            if goal, want one child to be 1
 *            if not goal, want one child to be 0
 *    
 */
int post_order_fix(node* tree, int pos, const bool goal) {
    int left, right, count = 0;

    if (tree[pos].g == node::gate::NONE) {
        return -1;
    }

    if (tree[pos].value == goal) {
        return 0;
    }

    std::tie(left, right) = left_right(pos);

    if (tree[pos].changeable) {
        if ((tree[right].value || tree[left].value) &&
        !(tree[right].value && tree[left].value)) {
            return 1;
        }
    }

    int a = post_order_fix(tree, left, goal);
    int b = post_order_fix(tree, right, goal);

    if (a != -1 || b != -1) {
        if (tree[pos].changeable) {
            return (a != -1 && b != -1 ? std::min(a, b) : std::max(a, b)) + 
            ((goal && tree[pos].g == node::gate::AND) ||
             (!goal && tree[pos].g == node::gate::OR) ? 1 : 0);
        }
        else {
            switch(tree[pos].g) {
                case node::gate::AND:
                    return goal ? (a != -1 && b != -1 ? a + b : -1) : 
                    (a != -1 && b != -1 ? std::min(a, b) : std::max(a, b));
                case node::gate::OR:
                    return goal ? (a != -1 && b != -1 ? std::min(a, b): std::max(a, b)) : 
                    (a != -1 && b != -1 ? a + b : -1);
                default:
                    break;
            }
        }
    }
    return -1;
}
