#include <iostream>
using namespace std;

int codeMap[] = {0, 1, 2, 3, 0, 1, 2, 0, 0, 2, 2,
             4, 5, 5, 0, 1, 2, 6, 2, 3, 0, 1,
             0, 2, 0, 2}, val, prev;

void codify(istream &iss)
{
    char out[39], *ptr = out, *cptr;
    out[38] = '\0';
    
    while(ptr < &out[9]) *ptr++ = ' ';
    
    for (ptr = &out[9]; iss.get(*ptr) ; ptr = &out[9])
    {
        prev = codeMap[*ptr - 'A'];
        out[34] = *ptr;
        
        for(cptr = &out[35]; *ptr && *ptr != '\n' && cptr < &out[38]; iss.get(*++ptr))
        {
            val = codeMap[*ptr - 'A'];
            if (val && val != prev)
                *cptr++ = val + '0';
            prev = val;         
        }
        while (cptr < &out[38]) *cptr++ = '0';
        while (*ptr && *ptr != '\n') iss.get(*++ptr);
        while (ptr < &out[34]) *ptr++ = ' ';
        cout << out << '\n';        
    }
}

int main()
{       
    cout << "         NAME                     SOUNDEX CODE\n";
    codify(cin);
    cout << "                   END OF OUTPUT\n";
    
    return 0;
}
