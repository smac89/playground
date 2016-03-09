#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#ifndef M_PI
#define M_PI (3.141592653589793)
#endif
#define EARTH_RADIUS (6370.0)
	
#define deg_to_rad(deg)(deg * M_PI / 180)

#ifdef DEBUG
#define MAX_SIZE 100
#else
#define MAX_SIZE 25
#endif


typedef int(*compare_args)(const void*, const void*, void **);
typedef struct {
	int lat, lon;
	struct Airport** adjlist;
	struct Airport* next;
} Airport;


Airport airports[MAX_SIZE];

double calculate_shortest_distance(int a1, int a2, int portcount, int fuel, int flightdist);

int main() {
	int ports, step, fd, q, s, e, c, a;
	double ans;

	for (step = 1; scanf("%d%d", &ports, &fd) != EOF; step++) {
		for (a = 0; a < ports; a++) {
			scanf("%d%d", &airports[a].lon, &airports[a].lat);
		}

		printf("Case %d:\n", step);

		for (scanf("%d", &q); q--; ) {
			scanf("%d%d%d", &s, &e, &c);
			ans = calculate_shortest_distance(s - 1, e - 1, ports, c, fd);
			if (ans == -1) {
				puts("impossible");
			}else {
				printf("%.3f\n", ans);
			}
		}
	}

	return 0;
}

#ifdef DEBUG
char* to_string(const Airport* ap) {
	char *string = malloc(10);
	sprintf(string, "%d, %df", ap->lat, ap->lon);
	return string;
}
#endif

/**
 * http://janmatuschek.de/LatitudeLongitudeBoundingCoordinates#Distance
 * @param  ap1 The first airpots
 * @param  ap2 The second airport
 * @return     The distance between the two airports
 */
double get_distance_between_airports(const Airport* ap1, const Airport* ap2) {
	double lat1 = deg_to_rad(ap1->lat);
	double lon1 = deg_to_rad(ap1->lon);
	double lat2 = deg_to_rad(ap2->lat);
	double lon2 = deg_to_rad(ap2->lon);

	return acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lon1 - lon2)) * EARTH_RADIUS;
}


/*#if (defined(__GNUC__) || defined(__GNUG__)) && !(defined(__clang__) || defined(__INTEL_COMPILER))
#endif*/

int compare(const void* ap1, const void* ap2, void** args) {
	int end = *(int*)(*args);
	const Airport* a1 = (const Airport*)(ap1);
	const Airport* a2 = (const Airport*)(ap2);

	double dist1 = get_distance_between_airports(a1, &airports[end]);
	double dist2 = get_distance_between_airports(a2, &airports[end]);
	printf("%d\n", end);
	return (int)(ceil(dist2 - dist1));
}

void sort(void* memory, size_t count, size_t elem_size, compare_args comp, int num_args, ...) {
	va_list arg_list;
	void **args = malloc(num_args * sizeof(void *));
	int a;

	int fake_compare(const void* t1, const void* t2) {
		return comp(t1, t2, args);
	}

	va_start(arg_list, num_args);

	for (a = 0; a < num_args; a++) {
		args[a] = va_arg(arg_list, void*);
	}

	va_end(arg_list);

	qsort(memory, count, elem_size, fake_compare);

	for (a = 0; a < num_args; a++) {
		free(args[a]);
	}
	free(args);
}

double calculate_shortest_distance(int a1, int a2, int portcount, int fuel, int flightdist) {

	sort(airports, portcount, sizeof (Airport), &compare, 1, a2 - 1);

	#ifdef DEBUG 
	{
		int j;
		for (j = 0; j < portcount; j++) {
			char *string = to_string(&airports[j]);
			puts(string);
			free(string);
		}
	}
	#endif
	return 0;
}
