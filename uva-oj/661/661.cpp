#include <iostream>
#define MAX(A, B) ((A ^ ((A ^ B) & -(A < B))))
using namespace std;

int main() 
{   
    char seq[] = {"Sequence "}, blow[] = {"Fuse was "},
    mp[] = {"Maximal power consumption was "};
    
    int devices[20], n, m, c, *ptr, test, power, d;
    
    for (test = 1; (cin >> n >> m >> c) && n > 0; ++test)
    {
        for (ptr = &devices[0]; n--; ++ptr) cin >> *ptr;
        
        for (power = 0; power <= c && m--; n = MAX(n, power))
        {
            cin >> d;
            power += devices[d-1];
            devices[d-1] *= -1;
        }
        
        cout << seq << test << '\n';
        if (m < 0)
        {
            cout << blow << "not blown.\n" << mp << n << " amperes.\n\n";
            continue;
        }
        while(m--) cin >> d;
        cout << blow << "blown.\n\n";
    }
    return 0;
}
