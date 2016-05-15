#include <iostream>
using namespace std;

inline unsigned stoU(const char *str_b, const char *str_e)
{
    unsigned x = 0;
    for ( const char *p = &str_b[0]; p != str_e; ++p )
        x = ( x * 10 ) + ( *p - '0' );
    return x;
}

inline double st_toM(const char *stime_t)
{
    return (double)(60 * stoU(&stime_t[0], &stime_t[2]) +
         stoU(&stime_t[3], &stime_t[5])) +
         (double)(stoU(&stime_t[6], &stime_t[8])) / 60; 
}

void solve()
{
    int kmh = 0;
    char time_t[10], c, km[] = {"km\n"};
    double prev_t = 0, dist = 0, temp;
    
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(2);
    
    while((cin >> time_t).get(c))
    {
        temp = st_toM(time_t);
        dist += kmh * (temp - prev_t) / 60;
        
        switch(c)
        {
            case '\n':
                cout << time_t << ' ' << dist << ' ' << km; break;
            default: cin >> kmh;
        }
        prev_t = temp;
    }
}


int main()
{   
    solve();
    return 0;
}
