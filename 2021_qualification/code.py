mapper = {}
class WordReader:
    def __init__(self, filename):
        with open(filename) as f:
            self.words = [word.strip() for line in f for word in line.split()]
            self.i = 0

    def __call__(self, type=int):
        self.i += 1
        return type(self.words[self.i - 1])

class Info:
    def __init__(self, file):
        wr = WordReader(f'2021_qualification/input/{file}')
        self.duration = wr()
        self.no_intersections = wr()
        self.no_streets = wr()
        self.no_cars = wr()
        self.bonus_per_car = wr()

        self.edges = []
        for _ in range(self.no_streets):
            from_id = wr()
            to_id = wr()
            name = wr(str)
            street_duration = wr()
            self.edges.append(Edge(from_id, to_id, name, street_duration))

        self.cars = []
        for _ in range(self.no_cars):
            path = [wr(str) for i in range(wr())]
            self.cars.append(Car(path))

class Car:
    def __init__(self, path):
        self.path = path

class Edge:
    def __init__(self, from_id, to_id, name, duration):
        self.from_id = from_id
        self.to_id = to_id
        self.name = name
        self.duration = duration
        self.deposit = 0
        self.ratio = 0

class Node:
    def __init__(self, node_id):
        self.node_id = node_id
        self.incoming = []
        self.deposit = 0
    
    def add_incoming(self, edge):
        self.incoming.append(edge)
    
    def calculate_deposit(self):
        for edge in self.incoming:
            self.deposit += edge.incoming.deposit

    def calculate_edge_ratio(self):
        for edge in self.incoming:
            if self.deposit != 0:
                edge.ratio = edge.deposit/self.deposit

class Graph:
    def __init__(self, edges, cars):
        self.nodes = {}
        self.edges = {}

        for edge in edges:
            self.edges[edge.name] = edge
            if edge.from_id not in self.nodes:
                self.nodes[edge.from_id] = Node(edge.from_id)

            if edge.to_id not in self.nodes:
                self.nodes[edge.to_id] = Node(edge.to_id)
            self.nodes[edge.to_id].add_incoming(edge)

        for car in cars:
            for edge_name in car.path:
                self.edges[edge_name].deposit += 1
        
        for node in self.nodes.values():
            for edge in node.incoming:
                node.deposit += edge.deposit
            node.calculate_edge_ratio()

def calculate_file(filename):
    info = Info(filename)

    g = Graph(info.edges, info.cars)

    GLOBAL_TIME = info.duration
    while GLOBAL_TIME >= 10:
        GLOBAL_TIME = GLOBAL_TIME/10

    with open(f"2021_qualification/output_py/{filename}", 'w+') as f:
        size = 0
        for node in g.nodes.values():
            if len(node.incoming) > 0:
                size += 1

        f.write(str(size))
        f.write('\n')

        for node in g.nodes.values():
            f.write(str(node.node_id))
            f.write('\n')
            f.write(str(len(node.incoming)))
            
            f.write('\n')
            for edge in node.incoming:
                to_write = int(GLOBAL_TIME*edge.ratio)
                if to_write == 0:
                    to_write = 1
                f.write(f'{edge.name} {str(to_write)}')
                f.write('\n')


files = ['a.txt', 'b.txt', 'c.txt', 'd.txt', 'e.txt', 'f.txt']

for file in files:
    calculate_file(file)