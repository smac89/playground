from GraphFromLink import MultiGraph
import sys
import random
import copy

class EulerianPath(object):
	def __init__(self, graph):
		self.graph = graph

	def find_eulerian_path(self):

		def findPath(fr, to, pathlist):
			pathlist.append(to)
			for w in self.graph.nodes[to]:
				if visited[to][w] > 0 and w != fr:
					visited[to][w] -= 1
					findPath(fr, w, pathlist)
				elif w == fr:
					pathlist.append(fr)
					break
			return pathlist

		# start from any point
		start = random.randint(1, self.graph.size)

		visited = copy.deepcopy(self.graph.nodes)

		path_exists = True

		return findPath(start, start, [])

if __name__ == '__main__':
	graph = MultiGraph.from_json(sys.argv[1])
	print (graph)
	print (EulerianPath(graph).find_eulerian_path())
	print (graph)
