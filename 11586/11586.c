#include <stdio.h>

int main() {
	int tests, track[2], temp;
	for (scanf("%d\n", &tests); tests--; ) {
		int m = 0, f = 0;
		while (1) {
			track[0] = getchar();
			track[1] = getchar();

			m += (track[0] == 'M') + (track[1] == 'M');
			f += (track[0] == 'F') + (track[1] == 'F');

			if (getchar() == '\n')
				break;
		}

		if (m > 1 && f > 1 && m == f) {
			puts("LOOP");
		} else {
			puts("NO LOOP");
		}
	}
}
