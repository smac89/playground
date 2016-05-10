#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "The program has to be run with a number as argument <%s num>", argv[0]);
		exit(1);
	}

	int val = 0, save, i;
	if (sscanf(argv[1], "%d", &val) && val >= 1) {
		save = val;
		for (i = 0; val != 1 && val > 1; i++) {
			if (val % 2 == 0) { // if the number is even
				val >>= 1;
			} else {
				val = (val << 1) + val + 1;
			}
		}

		if (val < 0) {
			printf("Detected integer overflow after %d steps!\n", i);
		} else {
			printf("The length of the collatz sequence starting at %d is %d\n", save, i);
		}
	} else if (val <= 0) {
		puts("The number has to be greater than 0");
	}

	return 0;
}
