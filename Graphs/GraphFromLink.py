import os
import re
import sys
import csv
import json
import collections

class Graph(object):
    FIELDNAMES = ['head', 'tail']
    DIRECTED = 1
    UNDIRECTED = 2
    __slots__ = ['nodes', 'size', 'graphtype']

    def __init__(self, size, graphtype):
        if size <= 0:
            raise ValueError("The size of the graph has to be greater than 0")
        self.nodes = [set() for _ in range(size + 1)]
        self.size = size
        self.graphtype = graphtype

    def add_edge(self, n1, n2):
        """
        Add an edge between n1 and n2
        """
        self.check_valid(n1)
        self.check_valid(n2)
        
        self.nodes[n1].add(n2)
        if self.graphtype == Graph.UNDIRECTED
            self.nodes[n2].add(n1)

    def has_edge(self, n1, n2):
        """
        Args:
            n1 (int): The id of the first vertex
            n2 (int): The id of the second vertex

        Returns:
            bool: True if n1 has an edge to n2
        """
        self.check_valid(n1)
        self.check_valid(n2)
        return n2 in self.nodes[n1]

    def check_valid(self, index):
        """
        Args:
            index (int): An index representing a single vertex in the graph

        Raises:
            ValueError: If the index given is not within the bounds of the graph
        """
        if index <= 0 or index > self.size:
            raise ValueError("The node does not exist: %d" %index)

    def parse_row(self, row):
        """
        Args:
            row (dict): A dictionary containing an edge in the graph
        """
        n1 = row['head']
        n2 = row['tail']
        if not self.has_edge(n1, n2):
            self.add_edge(int(n1), int(n2))

    def __repr__(self):
        return "%d\n%s\n" %(self.size, "\n".join("%d -> %d" %(n, len(self.nodes[n])) for n in range(1, self.size + 1)))

    def __str__(self):
        return "%d\n%s\n" %(self.size, "\n".join("%d -> %s" %(n, str(self.nodes[n])) for n in range(1, self.size + 1)))

    @staticmethod
    def on_error_exit(msg):
        sys.stderr.write(msg + '\n')
        sys.exit(1)

    @classmethod
    def from_csv(cls, fname):

        # if the file does not exist, don't try reading from it
        if os.path.isfile(fname):
            with open(fname, 'r') as csvfile:
                size = int(csvfile.readline().strip())

                graphtype = cls.UNDIRECTED
                direct = re.match(r"(false|true)", csvfile.readline().strip(), re.I)
                if direct and direct.group(0).lower() == "true":
                    graphtype = cls.DIRECTED

                graph_reader = csv.DictReader(csvfile, fieldnames=cls.FIELDNAMES)
                try:
                    graph = cls(size, graphtype)
                    for row in graph_reader:
                        graph.parse_row(row)
                    return graph
                except (ValueError, SyntaxError) as e:
                    Graph.on_error_exit(str(e))
        else:
            Graph.on_error_exit("Invalid file name: %s\nThe file does not exist or is not a file" %fname)

    @classmethod
    def from_json(cls, fname):

        # if the file does not exist, don't try reading from it
        if os.path.isfile(fname):
            with open(fname, 'r') as jsonfile:
                graph_dict = json.load(jsonfile)
                size = int(graph_dict["size"])
                graphtype = graph_dict["directed"]
                try:
                    graph = cls(size, graphtype)
                    for row in graph_dict["edges"]:
                        graph.parse_row(row)
                    return graph
                except (ValueError, SyntaxError) as e:
                    Graph.on_error_exit("%s\n" %str(e))
        else:
            Graph.on_error_exit("Invalid file name: %s\nThe file does not exist or is not a file\n" %fname)


    class Edge(collections.namedtuple('Edge', 'fromnode, tonode')):
        """This class can be used to represent edges in a graph

        Args:
            fromnode (int): The node which this edge starts from
            tonode (int): The node which this edge ends at
        """

        __slots__ = ()

        def __hash__(self):
            return hash((self.fromnode, self.tonode))

        def __eq__(self, other):
            if type(self) == type(other):
                return self.__dict__ == other.__dict__
            return False


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
        
        self.nodes[n1].add(WeightedGraph.EdgeTo(node=n2, weight=weight))

    


class MultiGraph(Graph):

    def __init__(self, size):
        super(MultiGraph, self).__init__(size)
        self.edgeList = []


    def add_edge(self, n1, n2):
        """
        Add an edge between n1 and n2
        """
        self.check_valid(n1)
        self.check_valid(n2)
        
        edge = MultiGraph.Edge(fromnode=n1, tonode=n2)
        self.edgeList.a(edge)
        self.
        if not self.nodes[n1].get(n2):
            self.nodes[n1][n2] = 0
        self.nodes[n1][n2] += 1

        g = 


if __name__ == '__main__':
    g = MultiGraph.from_json(sys.argv[1])
    print (g)

