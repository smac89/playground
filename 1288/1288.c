#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define PI (3.141592653589793)
#define EARTH_RADIUS (6370.0)
#define MAX_DIST_BETWEEN (1e9 + 7)
	
#define min(a,b)(a <= b ? a : b)
/*#define NDEBUG*/ /*Used to turn off assertions*/

#ifdef DEBUG
#define MAX_SIZE 50
#else
#define MAX_SIZE 25
#endif


/**
 * An attempt at C generic priority queue
 * @param  id   A unique identifier to be used for each instance of a priority queue
 * @param  type The type to store in the priority queue (Note that the queue stores array of
 * pointer to the type specified)
 * @return      A priority queue to be used with the type specified 
 */
#ifndef PRIORITY_Q_TEMPLATE
#define PRIORITY_Q_TEMPLATE(id, type) \
 /*Note that the ## is used to concantenate two names*/\
typedef int ( *pq_compare_##id) (const type*, const type*); \
\
typedef struct {\
	type **elements;\
	size_t size_pq, num_elem;\
	pq_compare_##id cmp;\
} PQ_##id;\
\
static PQ_##id* pq_init_##id(size_t cap, pq_compare_##id cmp) {\
	PQ_##id* p = malloc(sizeof (PQ_##id));\
	p->elements = malloc(sizeof (type*) * (cap + 1));\
	p->size_pq = cap + 1;\
	p->num_elem = 0;\
	p->cmp = cmp;\
	return p;\
}\
\
static void pq_push_##id(PQ_##id* pq, const type* item) {\
	/* If the number capacity of the queue has been reached, then this will print
	an error message */\
	if (pq->num_elem >= pq->size_pq) {\
		fputs("MAX CAPACITY OF Priority queue HAS BEEN REACHED!!!", stderr);\
		return;\
	}\
	/*I know this discards const-ness*/\
	pq->elements[++pq->num_elem] = item;\
	size_t best, currpos = pq->num_elem;\
	type* temp = NULL;\
\
	/*While the inserted element is better than it's parent, move it closer to the root*/\
	while (currpos > 1) {\
		if (pq->cmp(pq->elements[currpos], pq->elements[sibling(pq, currpos)]) < 0)\
			currpos = sibling(pq, currpos);\
		best = parent(pq, currpos);\
		if (pq->cmp(pq->elements[best], pq->elements[currpos]) < 0) {\
			temp = pq->elements[currpos];\
			pq->elements[currpos] = pq->elements[best];\
			pq->elements[best] = temp;\
		} else break;\
		currpos = best;\
	}\
}\
\
static type* pq_pop_##id(PQ_##id* pq) {\
	/*If the queue is empty*/\
	if (pq->num_elem == 0) {\
		fputs("Queue is empty", stderr);\
		return NULL;\
	}\
\
	type* top = pq->elements[1], *temp = NULL;\
	pq->elements[1] = pq->elements[pq->num_elem];\
	pq->elements[pq->num_elem--] = NULL;\
	size_t best, currpos = 1;\
	/*While the inserted element is less than one if it's children, swap it with the better child*/\
	while (currpos < pq->num_elem) {\
		if (pq->cmp(pq->elements[childl(pq, currpos)], pq->elements[childr(pq, currpos)]) < 0)\
			best = childr(pq, currpos);\
		else best = childl(pq, currpos);\
		if (pq->cmp(pq->elements[currpos], pq->elements[best]) < 0) {\
			temp = pq->elements[currpos];\
			pq->elements[currpos] = pq->elements[best];\
			pq->elements[best] = temp;\
		} else break;\
		currpos = best;\
	}\
	return top;\
}

#define sibling(pq, sib) (sib & 1 || sib == pq-> num_elem ? sib - 1 : sib + 1)
#define parent(pq, child) (child == 1 ? 1 : child >> 1)
/*The left child*/
#define childl(pq, parent) ((parent << 1) > pq->num_elem ? parent : (parent << 1))
/*The right child*/
#define childr(pq, parent) ((parent << 1) + 1 > pq->num_elem ? parent : (parent << 1) + 1)

#endif /*PRIORITY_Q_TEMPLATE*/

struct Airport {
	int lat, lon, edge_size;
	short id;
	struct Edge {
		double distTo;
		struct Airport* adj;
	}edges[MAX_SIZE];
} airports[MAX_SIZE];

PRIORITY_Q_TEMPLATE(airp, struct Airport);
void read_airports(int num);
double calculate_shortest_distance(int a1, int a2, int portcount, double max_dist);

int main() {
	int ports = 0, step = 0, q, s, e, a;
	double ans, c, fd;

	for (step = 1; scanf("%d%lf", &ports, &fd) != EOF; step++) {
		read_airports(ports);

		printf("Case %d:\n", step);

		for (scanf("%d", &q); q--; ) {
			scanf("%d%d%lf", &s, &e, &c);
			ans = calculate_shortest_distance(s - 1, e - 1, ports, min(c, fd));
			if (ans < 0) {
				puts("impossible");
			}else {
				printf("%.3lf\n", ans);
			}
		}
	}
	return 0;
}

void add_edge(int a1, int a2, double distTo) {
	struct Airport *w = &airports[a1], *v = &airports[a2];
	struct Edge *edge = &w->edges[w->edge_size++];
	edge->distTo = distTo;
	edge->adj = v;

	edge = &v->edges[v->edge_size++];
	edge->distTo = distTo;
	edge->adj = w;
}

void read_airports(int num) {
	int a;
	for (a = 0; a < num; a++) {
		scanf("%d%d", &airports[a].lon, &airports[a].lat);
		airports[a].edge_size = 0;
		airports[a].id = a;
	}
}

/**
 * http://janmatuschek.de/LatitudeLongitudeBoundingCoordinates#Distance
 * @param  ap1 The first airpots
 * @param  ap2 The second airport
 * @return     The distance between the two airports
 */
double get_distance_between_airports(const struct Airport* ap1, const struct Airport* ap2) {
	static double cache_this = PI / 180;
	static double double_earth = EARTH_RADIUS * 2;

	/*Convert each value to radians*/
	double lat1 = ap1->lat * cache_this,
	lat2 = ap2->lat * cache_this,
	lon1 = ap1->lon * cache_this,
	lon2 = ap2->lon * cache_this;

	double a = 0.5 - cos(lat2 - lat1) / 2 + cos(lat1) * cos(lat2) * (1 - cos(lon2 - lon1))/2;
  	return double_earth * asin(sqrt(a));
}

double dijkstra(const int start, const int end, const int num_elem) {
	double distTo[MAX_SIZE];
	int edge_start;
	struct Airport* top = &airports[start];
	struct Edge* edge;

	int diff_airports(const struct Airport* a1, const struct Airport* a2) {
		double d1 = get_distance_between_airports(a1, &airports[end]);
		double d2 = get_distance_between_airports(a2, &airports[end]);
		int sign = d1 < d2 ? -1 : 1;
		return (int)ceil(abs(d1 - d2)) * sign;
	}

	PQ_airp* pq = pq_init_airp(num_elem, diff_airports);

	/*Initialize the distance to every airport to a very large value*/
	for (edge_start = 0; edge_start < num_elem; edge_start++) {
		distTo[edge_start] = HUGE_VAL; /*This is macro defined in math.h*/
	}

	/*Using the bits of the number to determine if an airport has been visited or not*/
	long long visited = 0;

	/*Insert the first airport into the priority queue*/
	pq_push_airp(pq, top);

	/*Set the distance to that first airport to be 0*/
	distTo[top->id] = 0;

	while (pq->num_elem > 0) {
		do {
			top = pq_pop_airp(pq);
		while (((visited >> top->id)&1));
		
		if (top->id == end) {
			break;
		}

		/*printf("top id is %d, start is %d, end is %d\n", top->id, start, end);*/

		/*Mark the airport as visited*/
		visited |= (1 << top->id);

		for (edge_start = 0; edge_start < top->edge_size; edge_start++) {
			edge = &top->edges[edge_start];
			if (!((visited >> edge->adj->id) & 1)) {
				/*We only want to visit this airport if it makes the current distance to it
				any better*/
				if (distTo[edge->adj->id] > distTo[top->id] + edge->distTo) {
					distTo[edge->adj->id] = distTo[top->id] + edge->distTo;
					pq_push_airp(pq, edge->adj);
				}
			}
		}
	}

	return distTo[end] == HUGE_VAL ? -1 : distTo[end];
}

#ifdef DEBUG
char* to_string(const struct Airport* ap) {
	char *string = malloc(10);
	sprintf(string, "%d, %d", ap->lat, ap->lon);
	return string;
}
#endif

double calculate_shortest_distance(int a1, int a2, int portcount, double max_dist) {
	int x, y;
	double dist;

	for (x = 0; x < portcount; x++) {
		for (y = x + 1; y < portcount; y++) {
			dist = get_distance_between_airports(&airports[x], &airports[y]);
			if (dist <= max_dist) {
				add_edge(x, y, dist);
			}
		}
	}

	#ifdef DEBUG 
	{
		int i, j;
		char *string;
		for (j = 0; j < portcount; j++) {
			string = to_string(&airports[j]);
			printf("(%s)=>{", string);
			free(string);
			for (i = 0; i < airports[j].edge_size; i++) {
				string = to_string(airports[j].edges[i].adj);
				printf("(%s):%lf, ", string, airports[j].edges[i].distTo);
				free(string);
			}
			puts("}");
		}
	}
	#endif

	dist = dijkstra(a1, a2, portcount);
	return dist;
}
