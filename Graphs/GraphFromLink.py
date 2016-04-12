import os
import sys
import csv
import collections

class Graph(object):
	FIELDNAMES = ['head', 'tail']
	__slots__ = ['nodes', 'size']

	def __init__(self, size):
		if size <= 0:
			raise ValueError("The size of the graph has to be greater than 0")
		self.nodes = [set() for _ in range(size)]
		self.size = size

	def add_edge(self, n1, n2):
		"""
		Add an edge between n1 and n2
		"""
		self.check_valid_node(n1)
		self.check_valid_node(n2)
		
		self.nodes[n1 - 1].add(n2)

	def check_valid(self, index):
		if index <= 0 or index > self.size:
			raise ValueError("The node does not exist: %d" %index)

	def parse_row(self, row):
		n1 = row['head']
		n2 = row['tail']
		self.add_edge(int(n1), int(n2))

	def __repr__(self):
		return "%d\n%s\n" %(self.size, "\n".join("%d -> %d" %(n, len(self.nodes[n-1])) for n in range(self.size)))

	def __str__(self):
		return "%d\n%s\n" %(self.size, "\n".join("%d -> %s" %(n, str(self.nodes[n-1])) for n in range(self.size)))

	@classmethod
	def from_csv(cls, fname):

		# prints an error message and exits the program
		def on_error_exit(msg):
			sys.stderr.write(msg)
			sys.exit(1)

		# if the file does not exist, don't try reading from it
		if os.path.isfile(fname):
			with open(fname, 'rb') as csvfile:
				size = int(csvfile.readline().strip())
				graphReader = csv.DictReader(csvfile, fieldnames=cls.FIELDNAMES)
				try:
					graph = cls(size)
					for row in graphReader:
						graph.parse_row(row)
					return graph
				except (ValueError, SyntaxError) as e:
					on_error_exit("%s\n" %str(e))
		else:
			on_error_exit("Invalid file name: %s\nThe file does not exist or is not a file\n" %fname)



class WeightedGraph(Graph):
	FIELDNAMES = ['head', 'tail', 'weight']
	EdgeTo = collections.namedtuple('EdgeTo', 'node, weight')

	def parse_row(self, row):
		n1 = row['head']
		n2 = row['tail']
		w = row['weight']
		self.add_edge(int(n1), int(n2), float(w))

	def add_edge(self, n1, n2, weight):
		"""
		The edges are given by integers
		"""
		self.check_valid(n1)
		self.check_valid(n2)
		
		self.nodes[n1 - 1].add(WeightedGraph.EdgeTo(node=n2, weight=weight))

if __name__ == '__main__':
	g = WeightedGraph.from_csv(sys.argv[1])
	print (g)

