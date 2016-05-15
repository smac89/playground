#include <iostream>
using namespace std;

inline bool win(char *board, int *index)
{
    return '.' != board[index[0]] && 
         board[index[0]] == board[index[1]] &&
         board[index[1]] == board[index[2]];
}

void solve()
{
    int N, win_ind[][3] = {{0, 1, 2}, {3, 4, 5}, {0, 3, 6},
                     {2, 4, 6}, {1, 4, 7}, {6, 7, 8},
                     {2, 5, 8}, {0, 4, 8}}, X, O, (*ptr)[3], owin, xwin, t;
                         
    char board[9], res[][5] = {{"no\n"}, {"yes\n"}};
    board[9] = '\0';
    
    for (cin >> N; N--; )
    {
        ptr = &win_ind[0];
        X = O = owin = xwin = 0;
        
        for (t = 0; !(xwin && owin) && t < 9; ++t)
        {
            cin >> board[t];
            switch(board[t])
            {
                case 'X': ++X; break;
                case 'O': ++O; break;
                default: break;
            }
            
            while (ptr < &win_ind[8] && 2[*ptr] == t)
                if (win(board, *ptr++))
                    board[t] == 'X' ? xwin = 1 : owin = 1;
        }
        
        switch(xwin + owin)
        {
            case 0:
                cout << res[X == O || X - O == 1];
                break;
            case 1:
                cout << res[(xwin && X - O == 1) || (owin && O == X)];
                break;
            default:
                cout << res[0];
        }
        
        cin.ignore(9 - t, '\n');
    }
}

int main()
{
    solve();
    return 0;
}
