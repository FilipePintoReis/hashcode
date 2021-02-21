class WordReader:
    def __init__(self, filename):
        with open(filename) as f:
            self.words = [word.strip() for line in f for word in line.split()]
            self.i = 0

    def __call__(self, type=str):
        self.i += 1
        return type(self.words[self.i - 1])

class Pizza:
    def __init__(self, ingredients):
        self.ingredients = ingredients

    def __repr__(self):
        ret = ''
        for el in self.ingredients:
            ret += el + " "
        return ret

class Info:
    def __init__(self, number_2_p_teams, number_3_p_teams, number_4_p_teams, pizzas):
        self.number_2_p_teams = number_2_p_teams
        self.number_3_p_teams = number_3_p_teams
        self.number_4_p_teams = number_4_p_teams
        self.pizzas = pizzas

    def __repr__(self):
        return self.pizzas

reader = WordReader("training_round_2021/b_little_bit_of_everything.in")

no_pizzas = reader(int)
groups_2 = reader(int)
groups_3 = reader(int)
groups_4 = reader(int)

pizzas = []
for i in range(no_pizzas):
    ingredients = []
    for j in range(reader(int)):
        ingredients.append(reader())
    pizzas.append(Pizza(ingredients))

Info(groups_2, groups_3, groups_4, pizzas)

