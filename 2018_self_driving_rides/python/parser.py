class HashableList:
    def __init__(self, li):
        self.list = li

class WordReader:
        def __init__(self, filename, mapper = {}):
            '''
            This mapper is only useful if there are any strings.
            '''
            with open(filename) as f:
                self.words = [word.strip() for line in f for word in line.split()]
                self.i = 0

        def __call__(self, type=int):
            self.i += 1
            return type(self.words[self.i - 1])

class Ride:
    def __init__(self, from_x, from_y, to_x, to_y, e_start, l_end, index):
        self.from_x = from_x
        self.from_y = from_y
        self.to_x = to_x
        self.to_y = to_y
        self.e_start = e_start
        self.l_end = l_end
        self.index = index

        self.needed_steps = abs(from_x - to_x) + abs(from_y - to_y)
        self.l_start = l_end - self.needed_steps
        self.impossible = self.l_start <= 0

        # O actual start é quando eu dou o primeiro passo desta ride.
        self.actual_start = None
        self.actual_end = None

    def change_actual_start(self, value):
        self.actual_start = value
        self.calculate_actual_end()

    def calculate_actual_end(self):
        self.actual_end = self.actual_start + self.needed_steps

class Parser:
    def __init__(self, file):
        self.word_reader = WordReader(file)

        self.no_rows = self.word_reader()
        self.no_columns = self.word_reader()
        self.no_vehicles = self.word_reader()
        self.no_rides = self.word_reader()
        self.ride_bonus = self.word_reader()
        self.no_steps = self.word_reader()

        self.rides = [Ride(self.word_reader(),
                      self.word_reader(),
                      self.word_reader(),
                      self.word_reader(),
                      self.word_reader(),
                      self.word_reader(),
                      i) 
                      for i in range(self.no_rides)]

    def __call__(self):
        return Info(
            self.no_rows,
            self.no_columns,
            self.no_vehicles,
            self.no_rides,
            self.ride_bonus,
            self.no_steps,
            self.rides
        )

class Info:
    def __init__(self, 
                 no_rows,
                 no_columns,
                 no_vehicles,
                 no_rides,
                 ride_bonus,
                 no_steps,
                 rides):

        self.no_rows = no_rows
        self.no_columns = no_columns
        self.no_vehicles = no_vehicles
        self.no_rides = no_rides
        self.ride_bonus = ride_bonus
        self.no_steps = no_steps
        self.rides = rides
        self.heuristic = 0

        self.vehicle_rides = {vehicle:HashableList([])
                              for vehicle
                              in range(no_vehicles)}

        for index, ride in enumerate(rides):
            if ride.impossible:
                rides.remove(index)
                index -= 1
    
    def calc_heuristic(self):
        for vehicle_ride in self.vehicle_rides.values():
            for index, ride in enumerate(vehicle_ride.list):
                if ride.actual_start < ride.e_start:
                    self.heuristic = -1
                    return
                if index < len(vehicle_ride.list - 1):
                    for i in range(index + 1, len(vehicle_ride.list), 1):
                        if ride.actual_end >= vehicle_ride.list[i].actual_start:
                            self.heuristic = -1
                            return

            for i in range(len(vehicle_ride.list) - 1):
                first = vehicle_ride.list[i]
                second = vehicle_ride.list[i + 1]
                distance = abs(first.to_x - second.from_x) 
                distance += abs(first.to_y - second.from_y)

                if (second.actual_start - first.actual_end) > distance:
                    self.heuristic = -1
                    return
                if second.actual_end > self.no_steps:
                    self.heuristic = -1
                    return
            
            for ride in vehicle_ride.list:
                if ride.actual_end < ride.l_end:
                    self.heuristic += ride.needed_steps
                if ride.actual_start == ride.e_start:
                    self.heuristic += self.ride_bonus
                
    def starting_solution(self):
        for index, ride in enumerate(self.rides):
            self.vehicle_rides[index % (len(self.no_vehicles) - 1)].list.append(ride.index)
        for ride in self.rides:


    # def 
    





a_example = '2018_self_driving_rides/input/a_example.in'
b_should_be_easy = '2018_self_driving_rides/input/b_should_be_easy.in'
c_no_hurry = '2018_self_driving_rides/input/c_no_hurry.in'
d_metropolis = '2018_self_driving_rides/input/d_metropolis.in'
e_high_bonus = '2018_self_driving_rides/input/e_high_bonus.in'


p = Parser(a_example)
info = p()