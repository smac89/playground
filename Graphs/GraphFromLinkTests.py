#!/usr/bin
import unittest
import random
from GraphFromLink import Graph, WeightedGraph, MultiGraph

class GraphTest(unittest.TestCase):
    def test_edge_iscorrect(self):
        pass

    def setUp(self):
        self.graph_d = Graph(5, Graph.DIRECTED)
        self.graph_u = Graph(5, Graph.DIRECTED)
        
        GraphTest.fill_graph(graph_d, 8)
        GraphTest.fill_graph(graph_u, 5)

    @staticmethod
    def fill_graph(graph, numedge):
        for _ in range(numedge):
            n1, n2 = GraphTest.get_edge(graph):
            while graph.has_edge(n1, n2):
                n1, n2 = GraphTest.get_edge(graph):
            graph.add_edge(n1, n2)

    @staticmethod
    def get_edge(graph):
        n1 = random.randint(1, graph.size)
        n2 = random.randint(1, graph.size)
        if graph.size > 1:
            while n2 == n1:
                n2 = random.randint(1, graph.size)
        return (n1, n2)

