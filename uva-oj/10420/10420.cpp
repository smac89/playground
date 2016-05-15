#include <iostream>
#include <map>
using namespace std;

typedef map<string, int> msap;

void solve()
{
    int n;
    string country(76, '\0');
    msap count;
    typename msap::iterator it, fi; 
    
    for (cin >> n; n--; )
    {
        cin >> country;
        count[country]++;
        cin.ignore(76, '\n');
    }
    
    for (it = count.begin(), fi = count.end(); it != fi; ++it)
        cout << it->first << ' ' << it->second << '\n';
}

int main()
{
    solve();
    return 0;
}
