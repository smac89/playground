#include <iostream>
#include <map>
#define MAX(A, B)((A > B ? A : B))
#define MIN(A, B)(~MAX(~A, ~B))
#define EVEN(A) (~A & 1)
#define ll long long

int *ans;
std::map<ll, int> hasher;

inline int collatz(ll num)
{
    if (1 == num) return num;
    return *(ans = &hasher[num]) ? *ans :
        (*ans = -~collatz(EVEN(num) ? (num >> 1) :
        ((num << 1) + -~num)));
}

int solve(int a, int b) {
    int max(0);
    for (int t = a; t <= b; ++t)
        max = MAX(max, collatz(t));
    return max;
}

int main()
{
    int m, n;
    while (std::cin >> m >> n)
    std::cout << m << ' '
            << n << ' '
            << solve(MIN(m, n), MAX(m, n))
        << '\n';
    return 0;
}
