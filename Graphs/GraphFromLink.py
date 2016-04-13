import os
import re
import sys
import csv
import json
import collections

class Graph(object):
    """The base class for all graphs
    Attributes:
        nodes (list): A list that contains the edges between nodes
        size (int): The number of nodes in the graph
        graphtype (int): The type of links between the nodes (directed vs undirected)

        FIELDNAMES (list): The names of the columns in the csv file. Useful for reading
            the graph from a csv file
        DIRECTED (int): Number which implies that the graph has directed edges
        UNDIRECTED (int): Number which implies that the graph has undirected edges

    """

    FIELDNAMES = ['head', 'tail']
    UNDIRECTED, DIRECTED = range(2)
    __slots__ = ['edges', 'size', 'graphtype']

    def __init__(self, size, graphtype):
        """ Constructor for a graph

        Args:
            size (int): The number of nodes in the graph
            graphtype (int): The type of links between the nodes (directed vs undirected)
        """
        if size <= 0:
            raise ValueError("The size of the graph has to be greater than 0")
        if graphtype not in (Graph.DIRECTED, Graph.UNDIRECTED):
            raise ValueError("Unknown graph type given. Must be one of (DIRECTED: %d, UNDIRECTED %d"
                %(Graph.DIRECTED, Graph.UNDIRECTED))    

        self.edges = [set() for _ in range(size + 1)]
        self.size = size
        self.graphtype = graphtype

    def add_edge(self, n1, n2):
        """
        Add an edge between n1 and n2
        """
        self.check_valid(n1)
        self.check_valid(n2)
        
        self.edges[n1].add(n2)
        if self.graphtype == Graph.UNDIRECTED:
            self.edges[n2].add(n1)

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
        return n2 in self.edges[n1]

    def get_edges(self):
        """
        Returns:
            list: The list of edges in the graph
        """
        return [Graph.Edge(fromnode=u, tonode=v) for u in range(1, self.size + 1) for v in self.edges[u]]

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
        n1 = int(row['head'])
        n2 = int(row['tail'])
        if not self.has_edge(n1, n2):
            self.add_edge(n1, n2)

    def __repr__(self):
        return "%d\n%s\n" %(self.size, "\n".join("%d -> %d" %(n, len(self.edges[n])) for n in range(1, self.size + 1)))

    def __str__(self):
        return "%d\n%s\n" %(self.size, "\n".join("%d -> %s" %(n, str(self.edges[n])) for n in range(1, self.size + 1)))

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

                direct = re.match(r"(false|true)", csvfile.readline().strip(), re.I)

                graphtype = cls.DIRECTED if direct and direct.group(0).lower() == "true" else cls.UNDIRECTED

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
                graphtype = cls.DIRECTED if graph_dict["directed"] else cls.UNDIRECTED
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

        def __hash__(self):
            return hash((self.fromnode, self.tonode))

        def __eq__(self, other):
            if type(self) == type(other):
                return (self.fromnode, self.tonode) == (other.fromnode, other.tonode)
            elif tuple == type(other):
                return (self.fromnode, self.tonode) == other
            return False


class WeightedGraph(Graph):
    FIELDNAMES = ['head', 'tail', 'weight']

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
         
        self.edges[n1].add(WeightedGraph.WeightedEdge(weight, fromnode=n1, tonode=n2))
        if self.graphtype == Graph.UNDIRECTED:
            self.edges[n2].add(WeightedGraph.WeightedEdge(weight, fromnode=n2, tonode=n1))

    def get_edges(self):
        """
        Returns:
            list: The list of edges in the graph
        """
        return [edge for u in range(1, self.size + 1) for edge in self.edges[u]]

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
        return (n1, n2) in self.edges[n1]

    class WeightedEdge(Graph.Edge):
        # __slots__ = ('weight',)
        def __new__(cls, weight, **kwargs):
            self = super(WeightedGraph.WeightedEdge, cls).__new__(cls, **kwargs)
            self.weight = weight
            return self


class MultiGraph(Graph):

    def __init__(self, size, graphtype):
        super(MultiGraph, self).__init__(size, graphtype)
        self.edges = [dict() for _ in range(size + 1)]

    def parse_row(self, row):
        """
        Args:
            row (dict): A dictionary containing an edge in the graph
        """
        n1 = int(row['head'])
        n2 = int(row['tail'])
        self.add_edge(n1, n2)

    def add_edge(self, n1, n2):
        """
        Add an edge between n1 and n2
        """

        n = self.edges[n1].get(n2, 0) + 1
        self.edges[n1][n2] = n
        if self.graphtype == Graph.UNDIRECTED:
            n = self.edges[n2].get(n1, 0) + 1
            self.edges[n2][n1] = n

    def get_edges(self):
        """
        Returns:
            list: The list of edges in the graph
        """
        edges = [edge for edgecount in self.edges for (edge, count) in edgecount.items() for _ in range(count)]
        return edges

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
        return n2 in self.edges[n1]

if __name__ == '__main__':
    g = MultiGraph.from_json(sys.argv[1])
    print (g)

