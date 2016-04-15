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

    sc = graph[0][0]
    if sc: # If this edge does not lead to a dead end
        queue.append(sc)
        s, pos = switch_conn(sc)
        if pos != 1: # if it doesn't lead to an A connection
            switchTo[s - 1] = sc[-1] # Record how we got here
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

        s, p = switch_conn(graph[node][pos])
        # If the train goes through a fork
        if p != 1 and switchTo[s - 1] == None:
            # record which fork it took
            switchTo[s - 1] = graph[node][pos][-1]

        if pos == 0: # if we are at point A of a station
            sc = graph[node][1] # Try going through point B
            if sc: # If this edge does not lead to a dead end
                s, _ = switch_conn(sc)
                if not visited[s - 1]:
                    queue.append(sc)
                    visited[s - 1] = 1

            sc = graph[node][2] # Try going through point C
            if sc: # If this edge does not lead to a dead end
                s, _ = switch_conn(sc)
                if visited[s - 1] != 2:
                    queue.append(sc)
                    visited[s - 1] = 1
        else: # We are at point B or C which means only one way to go from here
            sc = graph[node][0]
            if sc: # If this edge does not lead to a dead end
                s, _ = switch_conn(sc)
                if not visited[s - 1]:
                    queue.append(sc)
                    switchTo[node] = parent[-1] # Record how we got here
                    visited[s - 1] = 1

    if found_cycle:
        return "".join(c if c else 'C' for c in switchTo)
    return "impossible"

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
