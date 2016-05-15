#include <stdio.h>

int main()
{   
    int found = 0, u;
    char op[] = {"``"}, cl[] = {"''"}, *ptr;
    while ((u = getchar()) != EOF)
    {
        if (u != '"')
        {
            putchar(u);
            continue;
        }
        ptr = found ? cl : op;
        putchar(ptr[0]);
        putchar(ptr[1]);
        found ^= 1;
    }
    return 0;
}
