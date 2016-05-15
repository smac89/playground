#include <stdio.h>
#define MODULO 42

int main() {
    int table [MODULO] = {}, n, ans = 0;
    scanf("%d", &n);
    table[n % MODULO] = 1;
    scanf("%d", &n);
    table[n % MODULO] = 1;
    scanf("%d", &n);
    table[n % MODULO] = 1;
    scanf("%d", &n);
    table[n % MODULO] = 1;
    scanf("%d", &n);
    table[n % MODULO] = 1;
    scanf("%d", &n);
    table[n % MODULO] = 1;
    scanf("%d", &n);
    table[n % MODULO] = 1;
    scanf("%d", &n);
    table[n % MODULO] = 1;
    scanf("%d", &n);
    table[n % MODULO] = 1;
    scanf("%d", &n);
    table[n % MODULO] = 1;

    for (n = 0; n < MODULO; n++) {
        ans += table[n];
    }

    printf("%d", ans);
    return 0;
}
