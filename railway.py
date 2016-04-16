import sys
import collections

EDGE_SIZE = 3

def bfs(graph):
    """The basic idea is to work backwards from the
    start until we get back to the start
    """
    queue = collections.deque()
    visited = [0 for _ in xrange(len(graph))]
    switchTo = [None for _ in xrange(len(graph))]

    def add_to_queue(con_str):
        s, p = switch_conn(con_str)
        parent, sw = switch_conn(graph[s - 1][p - 1])
        if sw != 1 and switchTo[parent - 1] == None:
            switchTo[parent - 1] = graph[s - 1][p - 1][-1]
        # print "Path to %s is %s" %(sc, graph[s - 1][p - 1])
        queue.append(con_str)
        visited[s - 1] = 1

    sc = graph[0][0]
    if sc: # If this edge does not lead to a dead end
        queue.append(sc)
    found_cycle = False

    while queue:
        parent = queue.popleft()
        node, pos = switch_conn(parent)
        node -= 1
        pos -= 1
        visited[node] = 2

        if node == 0: # if we reached station 1, we are finished
            found_cycle = True
            switchTo[0] = parent[-1]
            break

        if pos == 0: # if we are at point A of a station
            did_visit = False
            for i in (1,2):
                sc = graph[node][i] # Try going through a point
                if sc: # If this edge does not lead to a dead end
                    s, _ = switch_conn(sc)
                    if not visited[s - 1]:
                        did_visit = True
                        add_to_queue(sc)
            if not did_visit:
                sc = graph[node][0] # Try going through a point
                if sc: # If this edge does not lead to a dead end
                    s, _ = switch_conn(sc)
                    if not visited[s - 1]:
                        add_to_queue(sc)
                        switchTo[node] = parent[-1] # Record how we got here

        else: # We are at point B or C which means only one way to go from here
            did_visit = False
            sc = graph[node][0]
            if sc: # If this edge does not lead to a dead end
                s, p = switch_conn(sc)
                if not visited[s - 1]:
                    did_visit = True
                    add_to_queue(sc)
                    switchTo[node] = parent[-1] # Record how we got here

            if not did_visit:
                sc = graph[node][pos] # Try going through a point
                if sc: # If this edge does not lead to a dead end
                    s, _ = switch_conn(sc)
                    if not visited[s - 1]:
                        add_to_queue(sc)

    if found_cycle:
        return "".join(c if c else 'C' for c in switchTo)
    return "Impossible"

def make_edge(graph, start, end):
    switch_s, cp_s = switch_conn(start)
    switch_e, cp_e = switch_conn(end)

    graph[switch_s - 1][cp_s - 1] = end
    graph[switch_e - 1][cp_e - 1] = start

def switch_conn(con_str):
    return (int(con_str[:-1]), map_cp(con_str[-1]))

def map_cp(cp):
    return 1 if cp == 'A' else 2 if cp == 'B' else 3

if __name__ == '__main__':
    n, _ = map(int, raw_input().split())
    graph = [[None] * EDGE_SIZE for _ in xrange(n)]
    for line in sys.stdin.readlines():
        make_edge(graph, *line.strip().split())
    print bfs(graph)
