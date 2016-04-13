#!/usr/bin
import os
import unittest
import random
import datetime
import json
import collections
from GraphFromLink import Graph, WeightedGraph, MultiGraph

class TestWithGraph(object):
    def __init__(self, graphclass):
        self.graphclass = graphclass
    def __call__(self, cls):
        cls.GraphClass = property(lambda clself: self.graphclass)
        return cls

@TestWithGraph(Graph)
class GraphTestCase(unittest.TestCase):
    __metaclass__ = TestWithGraph

    NUM_NODES_IN_GRAPH = 5
    NUM_EDGES_UNDIR = 10
    NUM_EDGES_DIR = 8
    TestData = collections.namedtuple("TestData", "data, nodes, edges, directed")

    def test_1_should_raise_init(self):
        with self.assertRaises(ValueError):
            graph = self.GraphClass(0, Graph.DIRECTED)

    def test_2_edge_iscorrect(self):
        edges = self.graph_d.get_edges()
        self.assertEqual(len(edges), self.numedge_d,
            "Expected the number of edges added to be %d, but instead found %d!" %(self.numedge_d, len(edges)))

        edges = self.graph_u.get_edges()

        self.assertEqual(len(edges), self.numedge_u,
            "Expected the number of edges added to be %d, but instead found %d!" %(self.numedge_u, len(edges)))

    def test_3_can_read_csv(self):
        csv_input, numnodes, numedges, directed = self.get_csv_data()

        if not directed: numedges <<= 1

        #http://strftime.org/
        file_name = datetime.datetime.today().strftime("%c")
        GraphTestCase.write_csv_to_file(csv_input, file_name)

        graph = self.GraphClass.from_csv(file_name)
        edges = graph.get_edges()

        self.assertEqual(graph.size, numnodes,
            "Expected the graph to only contain %d nodes, found %d!" %(numnodes, graph.size))
        self.assertEqual(len(edges), numedges,
            "Expected the graph to only contain %d edges, found %d!" %(numedges, len(edges)))

    def test_4_can_read_json(self):
        json_data, numnodes, numedges, directed = self.get_json_data()

        if not directed: numedges <<= 1

        file_name = datetime.datetime.today().strftime("%c")
        GraphTestCase.write_json_to_file(json_data, file_name)

        graph = self.GraphClass.from_json(file_name)
        edges = graph.get_edges()

        self.assertEqual(graph.size, numnodes,
            "Expected the graph to only contain %d nodes, found %d!" %(numnodes, graph.size))
        self.assertEqual(len(edges), numedges,
            "Expected the graph to only contain %d edges, found %d!" %(numedges, len(edges)))

    def test_5_has_edge_test(self):
        numedges = len(self.graph_d.get_edges())
        n1, n2 = self.add_edge_to_graph(self.graph_d)[:2]

        self.assertTrue(self.graph_d.has_edge(n1, n2),
            "Expected the edge (%d, %d) to be in the graph!" %(n1, n2))

        self.assertEqual(len(self.graph_d.get_edges()), numedges + 1,
            "Expected %d edges, instead found %d" %(numedges + 1, len(self.graph_d.get_edges())))
        

    def setUp(self):
        self.graph_d = self.GraphClass(GraphTestCase.NUM_NODES_IN_GRAPH, Graph.DIRECTED)
        self.graph_u = self.GraphClass(GraphTestCase.NUM_NODES_IN_GRAPH, Graph.UNDIRECTED)
        self.numedge_d = GraphTestCase.NUM_EDGES_DIR
        self.numedge_u = GraphTestCase.NUM_EDGES_UNDIR

        for _ in range(self.numedge_d):
            self.add_edge_to_graph(self.graph_d)

        for _ in range(self.numedge_u >> 1):
            self.add_edge_to_graph(self.graph_u)

    def get_json_data(self):
        json_data = {
            "size":8,
            "directed": True,
            "edges": [
                {"head": 1, "tail": 5},
                {"head": 6, "tail": 2},
                {"head": 3, "tail": 4},
                {"head": 5, "tail": 8},
                {"head": 2, "tail": 7}
            ]
        }
        return GraphTestCase.TestData(data=json_data, nodes=8, edges=5, directed=True) 

    def get_csv_data(self):
        csv_input = "8\ntrue\n1,5\n6,2\n3,4\n5,8\n2,7"
        return GraphTestCase.TestData(data=csv_input, nodes=8, edges=5, directed=True)

    def add_edge_to_graph(cls, graph):
        n1, n2 = GraphTestCase.random_edge(graph)
        while graph.has_edge(n1, n2):
            n1, n2 = GraphTestCase.random_edge(graph)
        graph.add_edge(n1, n2)
        return (n1, n2)

    @classmethod
    def tearDownClass(cls):
        prefix = datetime.datetime.today().strftime("%a %b %d")
        for file in (f for f in os.listdir(os.curdir) if os.path.isfile(f)):
            if file.startswith(prefix):
                os.remove(file)

    @classmethod
    def random_edge(cls, graph):
        n1 = random.randint(1, graph.size)
        n2 = random.randint(1, graph.size)
        if graph.size > 1:
            while n2 == n1:
                n2 = random.randint(1, graph.size)
        return (n1, n2)

    @classmethod
    def write_json_to_file(cls, some_dict, file_name):
        with open (file_name, "w+") as test_json:
            test_json.write(json.dumps(some_dict))

    @classmethod
    def write_csv_to_file(cls, some_string, file_name):
        with open(file_name, "w+") as test_csv:
            test_csv.write(some_string)


@TestWithGraph(WeightedGraph)
class WeightedGraphTestCase(GraphTestCase):
    def get_json_data(self):
        json_data = {
            "size":8,
            "directed": True,
            "edges": [
                {"head": 1, "tail": 5, "weight": 10},
                {"head": 6, "tail": 2, "weight": 15},
                {"head": 3, "tail": 4, "weight": 10},
                {"head": 5, "tail": 8, "weight": 50},
                {"head": 2, "tail": 7, "weight": 10}
            ]
        }
        return WeightedGraphTestCase.TestData(data=json_data, nodes=8, edges=5, directed=True)

    def get_csv_data(self):
        csv_input = "8\ntrue\n1,5,10\n6,2,15\n3,4,10\n5,8,50\n2,7,10"
        return WeightedGraphTestCase.TestData(data=csv_input, nodes=8, edges=5, directed=True)

    def add_edge_to_graph(cls, graph):
        n1, n2 = WeightedGraphTestCase.random_edge(graph)
        while graph.has_edge(n1, n2):
            n1, n2 = WeightedGraphTestCase.random_edge(graph)
        weight = random.random() * int('F4CE',16)
        graph.add_edge(n1, n2, weight)
        return (n1, n2, weight)


@TestWithGraph(MultiGraph)
class MultiGraphTestCase(GraphTestCase):
    pass

if __name__ == '__main__':
    suite = unittest.TestLoader().loadTestsFromTestCase(GraphTestCase)
    unittest.TextTestRunner(verbosity=2).run(suite)

    suite = unittest.TestLoader().loadTestsFromTestCase(WeightedGraphTestCase)
    unittest.TextTestRunner(verbosity=2).run(suite)

    suite = unittest.TestLoader().loadTestsFromTestCase(MultiGraphTestCase)
    unittest.TextTestRunner(verbosity=2).run(suite)

