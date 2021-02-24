import sys

class WordReader:
        def __init__(self, filename, mapper = {}):
            '''
            This mapper is only useful if there are any strings.
            '''
            with open(filename) as f:
                self.words = [word.strip() for line in f for word in line.split()]

                # no = 0
                # for word in self.words:
                #     try:
                #         int(word)
                #     except:
                #         mapper[word] = no
                #         no += 1

                self.i = 0

        def __call__(self, type=int):
            self.i += 1
            return type(self.words[self.i - 1])

class Ride:
    def __init__(self, from_x, from_y, to_x, to_y, e_start, l_finish):
        self.from_x = from_x
        self.from_y = from_y
        self.to_x = to_x
        self.to_y = to_y
        self.e_start = e_start
        self.l_finish = l_finish

class Parser:
    def __init__(self, file):
        self.word_reader = WordReader(file)

        self.no_rows = self.word_reader()
        self.no_columns = self.word_reader()
        self.no_vehicles = self.word_reader()
        self.no_rides = self.word_reader()
        self.ride_bonus = self.word_reader()
        self.no_steps = self.word_reader()

        rides = [Ride(self.word_reader(),
                      self.word_reader(),
                      self.word_reader(),
                      self.word_reader(),
                      self.word_reader(),
                      self.word_reader()) 
                      for _ in range(no_rides)]


class Info:
    def __init__(self, 
                 no_rows,
                 no_columns,
                 no_vehicles,
                 no_rides,
                 ride_bonus,
                 no_steps):

        self.no_rows = no_rows
        self.no_columns = no_columns
        self.no_vehicles = no_vehicles
        self.no_rides = no_rides
        self.ride_bonus = ride_bonus
        self.no_steps = no_steps

        self.vehicle_map = {(vehicle, [0,0]) 
                            for vehicle 
                            in range(no_vehicles)}

a_example = '2018_self_driving_rides/input/a_example.in'
b_should_be_easy = '2018_self_driving_rides/input/b_should_be_easy.in'
c_no_hurry = '2018_self_driving_rides/input/c_no_hurry.in'
d_metropolis = '2018_self_driving_rides/input/d_metropolis.in'
e_high_bonus = '2018_self_driving_rides/input/e_high_bonus.in'


p = Parser(a_example)