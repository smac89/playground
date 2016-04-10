import re
from collections import namedtuple

class Node(object):
	__slots__ = ['name', 'id']

	def __init__(self, ident, name=None):
		if name is not None:
			self.name = name
		else: self.name = str(ident)
		self.id = ident

	def __repr__ (self):
		"""
		calling the print function on this object will call this function
		"""
		return self.name

	def __str__(self):
		"""
		Calling the str method on this object will use this method
		"""
		return "id=%s, name=%s" % (str(self.id), self.name)

	def __hash__(self):
		return hash((self.name, self.id))

	def __eq__(self, other):
		if type(other) is type(self):
			return self.__dict__ == other.__dict__
		return False


class Graph(object):
	PATTERNS = [re.compile(r'.*?\((.+?)\)(?:->\((.+?)\)\s*)?'),
		re.compile(r'.*?\((.+?)\)(?:<->\((.+?)\)\s*)?')]
	DIRECTED, UNDIRECTED = range(2)
	__slots__ = ['nodes', 'size', 'pattern']

	def __init__(self):
		self.nodes = dict()
		self.size = 0
		self.pattern = PATTERNS[DIRECTED]

	def add_edge(self, n1, n2):
		"""
		Add an edge between n1 and n2
		"""
		if n1.lower() not in self.nodes:
			n1 = Node(self.size + 1, n1)
		else: n1 = self.nodes[n1.lower()]
			n1 = Node()

		# self.check_valid_node(n1.ident)
		# self.check_valid_node(n2.ident)
		
		# self.nodes[n1] = n2
		pass


	def parse_line(self, line):
		match = self.pattern.search(line)
		if match:
			components = match.groups()
			return namedtuple('Edge', 'head, tail, weight')
			
			return components
		raise SyntaxError("Invalid syntax on input: (%s)" %line)

	@classmethod
	def from_file(cls, f):
		with f:
			try:
				# create the graph from the class we are given
				graph = cls()

				# determine what kind of edges we have
				fst = f.readline().strip()
				match = graph.pattern.search(fst)

				# if it doesn't match what we are expecting, switch to the other
				# pattern
				if not match: graph.pattern = cls.PATTERNS[cls.UNDIRECTED]
				names = {}
				num = 1

				graph.add_edge(*graph.parse_line(fst))
				for line in f:
					line = line.strip()
					if line:
						edge = graph.parse_line(line)
						if edge[0].lower() not in names:
							names[edge[0].lower()] = Node(num, edge[0])


						lst = names.get(, [])
						lst.append(components)
						names
						names[components[0]] = components



			except (ValueError, SyntaxError) as e:
				print (e)

class WeightedGraph(Graph):
	pattern = re.compile(r'.*?\((.+?)\)(?:->\((.+?)\)\s*([-+]?\d*\.?\d+(?:[eE][-+]?\d+)?))?')
	def add_edge(self, n1, n2, weight):
		"""
		The edges are given by integers
		"""
		# self.check_valid_node(n1.ident)
		# self.check_valid_node(n2.ident)
		
		# self.nodes[n1] = (n2, weight)
		pass

if __name__ == '__main__':
	g = WeightedGraph.from_file(open('somefile', 'r'))

