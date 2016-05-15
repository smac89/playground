#include <iostream>
using namespace std;

inline void wertyu(char &ch)
{
    switch (ch) 
    {
        case '1': cout.put('`'); break;
        case '2': cout.put('1'); break;
        case '3': cout.put('2'); break;
        case '4': cout.put('3'); break;
        case '5': cout.put('4'); break;
        case '6': cout.put('5'); break;
        case '7': cout.put('6'); break;
        case '8': cout.put('7'); break;
        case '9': cout.put('8'); break;
        case '0': cout.put('9'); break;
        case '-': cout.put('0'); break;
        case '=': cout.put('-'); break;
        case 'W': cout.put('Q'); break;
        case 'S': cout.put('A'); break;
        case 'X': cout.put('Z'); break;
        case 'E': cout.put('W'); break;
        case 'D': cout.put('S'); break;
        case 'C': cout.put('X'); break;
        case 'R': cout.put('E'); break;
        case 'F': cout.put('D'); break;
        case 'V': cout.put('C'); break;
        case 'T': cout.put('R'); break;
        case 'G': cout.put('F'); break;
        case 'B': cout.put('V'); break;
        case 'Y': cout.put('T'); break;
        case 'H': cout.put('G'); break;
        case 'N': cout.put('B'); break;
        case 'U': cout.put('Y'); break;
        case 'J': cout.put('H'); break;
        case 'M': cout.put('N'); break;
        case 'I': cout.put('U'); break;
        case 'K': cout.put('J'); break;
        case 'O': cout.put('I'); break;
        case 'L': cout.put('K'); break;
        case 'P': cout.put('O'); break;
        case '\\': cout.put(']'); break;
        case ']': cout.put('['); break;
        case '[': cout.put('P'); break;
        case '\'': cout.put(';'); break;
        case ';': cout.put('L'); break;
        case '/': cout.put('.'); break;
        case '.': cout.put(','); break;
        case ',': cout.put('M'); break;
        default: cout.put(ch); break;
    }
}

int main()
{
    char v; 
    while(cin.get(v)) wertyu(v);
    return 0;
}
