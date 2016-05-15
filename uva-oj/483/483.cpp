#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

inline void puts(char n) { cout.put(n); }

int main() {
    string word;
    char space;
    while ((cin >> word).get(space))
    {
        for_each(word.rbegin(), word.rend(), puts);
        cout.put(space);
    }
    return 0;
}
