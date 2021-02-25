class WordReader:
    def __init__(self, filename):
        with open(filename) as f:
            self.words = [word.strip() for line in f for word in line.split()]
            self.i = 0

    def __call__(self, type=str):
        self.i += 1
        return type(self.words[self.i - 1])

class Info:
    def __init__(self, file):
        wr = WordReader(f'2021_qualification/{file}')
        self.duration = wr()
        self.no_intersections = wr()
        self.no_streets = wr()
        self.no_cars = wr()
        self.bonus_per_car = wr()

        edges = []
        for i in range(self.no_streets):
            from_id = wr()
            to_id = wr()
            name = wr()
            street_duration = wr()
            edges.append(Edge(from_id, to_id, name, street_duration))

        for i in range(self.no_cars):
            car_path_street_names = [wr() for i in wr()]

class Car:
    def __init__(self, a):
        pass

class Edge:
    def __init__(self, from_id, to_id, name, duration):
        self.from_id = from_id
        self.to_id = to_id
        self.name = name
        self.duration = duration
        self.deposit = 0

class Node:
    def __init__(self):
        self.incoming = []
    
    def add_incoming(self, name):
        self.incoming.append(name) 

class Graph:
    def __init__(self, edges):

        

# mapper = {}
# class WordReader:
#     def __init__(self, filename, mapper):
#         with open(filename) as f:
#             self.words = [word.strip() for line in f for word in line.split()]

#             no = 0
#             for word in self.words:
#                 try:
#                     int(word)
#                 except:
#                     mapper[word] = no
#                     no += 1

#             self.i = 0

#     def __call__(self, type=str):
#         self.i += 1
#         return type(self.words[self.i - 1])

