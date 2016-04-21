import sys
import random
import copy
from GraphFromLink import MultiGraph

class EulerianPath(object):
	def __init__(self, graph):
		self.graph = graph

	def find_eulerian_path(self):

		def findPath(fr, to, pathlist):
			for (v, c) in self.graph.edges[to].items():
				if visited[(to, v)] > 0 and visited[(to, v)] == prev_visited[(to, v)] and v != fr:
					visited[(to, v)] -= 1
					if self.graph.graphtype == MultiGraph.UNDIRECTED:
						visited[(v, to)] -= 1
					findPath(fr, v, pathlist)
					if pathlist:
						pathlist.append(v)
						break
				elif visited[(to, v)] > 0 and v == fr:
					pathlist.append(fr)
					break
			return pathlist

		unused = set(range(1, self.graph.size + 1))
		visited = {(u, v): count for (u, dedge) in enumerate(self.graph.edges) for (v, count) in dedge.items()}
		pathlist = []

		start = unused.pop()

		while True:
			prev_visited = copy.deepcopy(visited)
			path = findPath(start, start, [])
			if len(path) > 1:
				if pathlist.count(start) > 0:
					ind = pathlist.index(start)
					pathlist = pathlist[:ind] + path + pathlist[ind + 1:]
				else: pathlist.extend(path)
			elif not unused: break
			start = unused.pop()

		return pathlist

if __name__ == '__main__':
	graph = MultiGraph.from_json(sys.argv[1])
	print (graph)
	print (EulerianPath(graph).find_eulerian_path())
	print (graph)
