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
#define min(a,b)(a <= b ? a : b)

#ifdef DEBUG
#define MAX_SIZE 100
#else
#define MAX_SIZE 25
#endif

#define extra_args(args...) args
#define str(s...) #s


typedef int(*compare_args)(const void*, const void*, void **);

typedef struct {
	int lat, lon;
	int edge_size;
	struct Airport** edges;
} Airport;


Airport airports[MAX_SIZE];

double calculate_shortest_distance(int a1, int a2, int portcount, int max_dist);

int main() {
	int ports = 0, step = 0, fd = 0, q, s, e, c, a;
	double ans;

	puts(extra_args(ports, step, fd));

	for (step = 1; scanf("%d%d", &ports, &fd) != EOF; step++) {
		for (a = 0; a < ports; a++) {
			scanf("%d%d", &airports[a].lon, &airports[a].lat);
		}

		printf("Case %d:\n", step);

		for (scanf("%d", &q); q--; ) {
			scanf("%d%d%d", &s, &e, &c);
			ans = calculate_shortest_distance(s - 1, e - 1, ports, min(c, fd));
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


void add_edge(int a1, int a2) {

}

double calculate_shortest_distance(int a1, int a2, int portcount, int max_dist) {
	int x, y, dist;

	for (x = 0; x < portcount; x++) {
		for (y = x + 1; y < portcount; y++) {
			dist = (int)ceil(get_distance_between_airports(&airports[x], &airports[y]));
			if (dist <= max_dist) {

			}
		}
	}

	sort(airports, portcount, sizeof (Airport), &compare, 1, a2 - 1);

	extra_args(a1, a2, fuel);

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
