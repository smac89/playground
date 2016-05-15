#include <stdio.h>
void solve()
{
    const long MS_DAY = 108;
    const long DC_DAY = 125;
    char in[10];
    int hh, mm, ss, cc, tt;
    while(gets(in))
    {
        hh = (in[0] - '0') * 10 + (in[1] - '0');
        mm = (in[2] - '0') * 10 + (in[3] - '0');
        ss = (in[4] - '0') * 10 + (in[5] - '0');
        cc = (in[6] - '0') * 10 + (in[7] - '0');
        tt = (3600 * hh + mm * 60 + ss) * 100 + cc;
        tt = tt * DC_DAY / MS_DAY;
        printf("%07d\n", tt);
    }
}

int main()
{   
    solve();
    return 0;
}
