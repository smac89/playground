#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef M_PI
#define M_PI (3.141592653589793)
#endif
#define EARTH_RADIUS (6370.0)
	
#define deg_to_rad(deg)(deg * M_PI / 180)
#define min(a,b)(a <= b ? a : b)
/*#define NDEBUG*/ /*Used to turn off assertions*/

#ifdef DEBUG
#define MAX_SIZE 50
#else
#define MAX_SIZE 25
#endif


struct Airport {
	int lat, lon;
	int edge_size;
	struct Airport* edges[MAX_SIZE];
} airports[MAX_SIZE];


void read_airports(int num);
double calculate_shortest_distance(int a1, int a2, int portcount, int max_dist);

int main() {
	int ports = 0, step = 0, fd = 0, q, s, e, c, a;
	double ans;

	for (step = 1; scanf("%d%d", &ports, &fd) != EOF; step++) {
		read_airports(ports);

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

typedef int ( *pq_compare) (const void*, const void*, ...); 
 
typedef struct { 
	void **elements;
	size_t size_pq, num_elem;
	pq_compare cmp;
} PQ;

static PQ* pq_init(size_t cap, pq_compare cmp) {
	PQ* p = malloc(sizeof (PQ));
	p->elements = malloc(sizeof (void**) * (cap + 1));
	p->size_pq = cap + 1;
	p->num_elem = 0;
	p->cmp = cmp;
	return p;
}
 
static void pq_insert(PQ* pq, void* item) {
	/* If the number capacity of the queue has been reached, then this will print
	an error message */
	if (pq->num_elem >= pq->size_pq) {
		fputs("MAX CAPACITY OF Priority queue HAS BEEN REACHED!!!", stderr);
		return;
	}
	size_t best, currpos = pq->num_elem;
	void* temp = NULL;
	pq->elements[++pq->num_elem] = item;

	while (currpos > 1) {
		if (pq->cmp(pq->elements[currpos >> 1], pq->elements[(currpos >> 1) + 1]) < 0)
			best = (currpos >> 1) + 1;
		else best = currpos >> 1;
		if (pq->cmp(pq->elements[currpos], pq->elements[best]) < 0) {
			temp = pq->elements[currpos];
			pq->elements[currpos] = pq->elements[best];
			pq->elements[best] = temp;
		}
		currpos = best;
	}
}

static void* pq_remove_best(PQ* pq) {
	if (pq->num_elem == 0) {
		fputs("Queue is empty", stderr);
		return NULL;
	}

	if (pq->num_elem == 1) {
		free(pq->elements[1]);
		pq->elements[1] = NULL;
		return NULL
	}

	pq->elements[1] = pq->elements[num_elem--];
	
}

int diff_airports(const struct Airport* a1, const struct Airport* a2, int t) {
	return 0;
}

#ifdef DEBUG
char* to_string(const struct Airport* ap) {
	char *string = malloc(10);
	sprintf(string, "%d, %d", ap->lat, ap->lon);
	return string;
}
#endif


void add_edge(int a1, int a2) {
	struct Airport *w = &airports[a1], *v = &airports[a2];
	w->edges[w->edge_size++] = v;
	v->edges[v->edge_size++] = w;
}

void read_airports(int num) {
	int a;
	for (a = 0; a < num; a++) {
		scanf("%d%d", &airports[a].lon, &airports[a].lat);
		airports[a].edge_size = 0;
	}
}


void dijkstra(int start, int end) {
	PQ* pq = pq_init(20, diff_airports);
}

/**
 * http://janmatuschek.de/LatitudeLongitudeBoundingCoordinates#Distance
 * @param  ap1 The first airpots
 * @param  ap2 The second airport
 * @return     The distance between the two airports
 */
double get_distance_between_airports(const struct Airport* ap1, const struct Airport* ap2) {
	static int func;

	double lat1 = deg_to_rad(ap1->lat);
	double lon1 = deg_to_rad(ap1->lon);
	double lat2 = deg_to_rad(ap2->lat);
	double lon2 = deg_to_rad(ap2->lon);

	return acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lon1 - lon2)) * EARTH_RADIUS;
}

double calculate_shortest_distance(int a1, int a2, int portcount, int max_dist) {
	int x, y, dist;

	for (x = 0; x < portcount; x++) {
		for (y = x + 1; y < portcount; y++) {
			dist = (int)ceil(get_distance_between_airports(&airports[x], &airports[y]));
			if (dist <= max_dist) {
				add_edge(x, y);
			}
		}
	}

	dijkstra(a1, a2);

	#ifdef DEBUG 
	{
		int j;
		char *string;
		for (j = 0; j < portcount; j++) {
			string = to_string(&airports[j]);
			puts(string);
			free(string);
		}
	}
	#endif
	return 0;
}
