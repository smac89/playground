#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define PI (3.141592653589793)
#define EARTH_RADIUS (6370.0)
#define TO_RAD (PI / 180)
#define TO_DEG (180 / PI)

#define MIN_LAT	(-90 * (TO_RAD)) /* -PI / 2*/
#define MAX_LAT (90 * (TO_RAD)) /* PI / 2*/
#define MIN_LON (-180 * (TO_RAD)) /* -PI*/
#define MAX_LON (180 * (TO_RAD)) /* PI*/

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

typedef unsigned char byte;

/**
 * Swaps the contents of two memory locations. This function assumes there is no
 * overlap between memory1 and memory2
 * This function is unsafe because it does not check memory bounds
 * @param memory1   The start of the first memory address
 * @param memory2   The start of the second memory address
 * @param num_bytes The size in bytes of the amount of memory to swap
 */
void unsafe_memswap(void *memory1, void *memory2, size_t num_bytes) {
	register byte * p1 = (byte *)(memory1);
	register byte * p2 = (byte *)(memory2);
	byte run;
	if (memory1 != memory2) {
		while (num_bytes --) {
			run = *p1;
			*p1++ = *p2;
			*p2++ = run;
		}
	}
}

#endif /*PRIORITY_Q_TEMPLATE*/





#ifndef CONNECTED_COMPONENT
#define CONNECTED_COMPONENT(id) \
typedef size_t type; \
typedef struct { \
	type *cc; \
} CC_##id; \
\
static CC_##id* cc_init_##id(size_t size) { \
	CC_##id *component = malloc(sizeof (CC_##id)); \
	component->cc = malloc(sizeof (type) * size + 1); \
	type i; \
	for (i = 1; i <= size; i++) { \
		/* Initially everything is in it's own connected component*/ \
		component->cc[i] = i; \
	} \
	return component; \
} \
\
type CC_find_##id (CC_##id* component, type child) { \
	if (component->cc[child] == child) { \
		return child; \
	} \
	/* Compress the path */ \
	component->cc[child] = component->cc[component->cc[child]]; \
	return CC_find_##id(component, component->cc[child]); \
} \
\
void CC_union_##id (CC_##id* component, type c1, type c2) { \
	component->cc[CC_find_##id(component, c1)] = CC_find_##id(component, c2); \
}\
\
int CC_isconnected_##id (CC_##id* component, type c1, type c2) { \
	return CC_find_##id(component, c1) == CC_find_##id(component, c2); \
}
#endif /*CONNECTED_COMPONENT*/


struct node {
	short id;
	double rad_lat, rad_lon;
	int edge_size
	unsigned long long has_edge_to;
	struct edge {
		struct node *adj;
	} edges[MAX_SIZE];
} nodes[MAX_SIZE];

struct Airport {
	int edge_size;
	short id;
	struct GeoLoc{
		int lat, lon;
		double rad_lat, rad_lon;
	}loc;

	unsigned long long has_edge_to;
	struct Edge {
		double distTo;
		struct Airport* adj;
	}edges[MAX_SIZE];
} airports[MAX_SIZE];

PRIORITY_Q_TEMPLATE(airp, struct Airport);
CONNECTED_COMPONENT(airp);
void read_airports(int num);
double calculate_shortest_distance(int a1, int a2, int portcount, double fuel_cap, double flight_dist);


/**
 * Main entry point
 * @return program exit status
 */
int main() {
	int ports = 0, step = 0, q, s, e, a;
	double ans, c, fd;

	for (step = 1; scanf("%d%lf", &ports, &fd) != EOF; step++) {
		read_airports(ports);

		printf("Case %d:\n", step);

		for (scanf("%d", &q); q--; ) {
			scanf("%d%d%lf", &s, &e, &c);
			ans = calculate_shortest_distance(s - 1, e - 1, ports, c, fd);
			if (ans < 0) {
				puts("impossible");
			}else {
				printf("%.3lf\n", ans);
			}
		}
	}
	return 0;
}

void add_edge(int a1, int a2) {
	struct node *w = &nodes[a1], *v = &nodes[a2];

	/*If there is already an edge between these 2, then don't add a new edge*/
	if (w->has_edge_to & (1ULL << v->id)) {
		return;
	}
	struct edge *edge = &w->edges[w->edge_size++];
	edge->adj = v;


	edge = &v->edges[v->edge_size++];
	edge->adj = w;

	/*Mark that there exists an edge between the airports*/
	w->has_edge_to |= (1ULL << v->id);
	v->has_edge_to |= (1ULL << w->id);
}

void read_airports(int num) {
	int a, lon, lat;
	struct node* ap;
	for (a = 0; a < num; a++) {
		scanf("%d%d", &lon, &lat);
		lon -= 180;
		ap = &noodes[a];
		ap->rad_lat = lat * TO_RAD;
		ap->rad_lon = lon * TO_RAD;

		ap->edge_size = 0;
		ap->id = a;
		ap->has_edge_to = 0ULL; /*Has to edge to no one initially*/
	}
}

int intersects_circle(const struct node* a, const struct node* b, const double radius) {
	double diffx = a->rad_lat - b->rad_lat;
	double diffy = a->rad_lon - b->rad_lon;
	double length = sqrt(diffx * diffx + diffy * diffy);
	return length <= radius * 2;
}

/**
 * http://janmatuschek.de/LatitudeLongitudeBoundingCoordinates#Distance
 * @param  ap1 The first airpots
 * @param  ap2 The second airport
 * @return     The distance between the two airports
 */
double get_distance_between_airports(const struct node* ap1, const struct node* ap2) {
	static double double_earth = EARTH_RADIUS * 2;

	double a = 0.5 - cos(ap2->rad_lat - ap1->rad_lat) / 2 + 
	cos(ap1->rad_lat) * cos(ap2->rad_lat) * (1 - cos(ap2->rad_lon - ap1->rad_lon))/2;
  	return double_earth * asin(sqrt(a));
}

double short_path(const int start, const int end, const int num_elem) {
	double distTo[MAX_SIZE];
	int edge_start;
	struct node* top = &nodes[start];
	struct edge* edge;

	int diff_airports(const struct node* a1, const struct node* a2) {
		double d1 = get_distance_between_airports(a1, &nodes[end]);
		double d2 = get_distance_between_airports(a2, &nodes[end]);
		double diff = d1 - d2;
		return diff < 0 ? -1 : diff > 0 ? 1 : 0;
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
		}while (((visited >> top->id)&1));

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
	sprintf(string, "%d, %d", ap->loc.lat, ap->loc.lon);
	return string;
}
#endif

double calculate_shortest_distance(int a1, int a2, int portcount, double fuel_cap, double flight_dist) {
	int w, x, y, z;
	double dist;
	struct node *temp, *array[MAX_SIZE];
	struct GeoLoc boundBox[2];

	for (x = 0; x < portcount; x++) {
		for (y = x + 1; y < portcount; y++) {
			dist = get_distance_between_airports(&nodes[x], &nodes[y]);
			if (dist <= flight_dist) {
				if (intersects_circle(&nodes[x], &nodes[y], flight_dist)) {
					add_edge(x, y);
				}
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

	dist = short_path(a1, a2, portcount);
	return dist;
}
