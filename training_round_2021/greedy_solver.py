import random

mapper = {}
class WordReader:
    def __init__(self, filename, mapper):
        with open(filename) as f:
            self.words = [word.strip() for line in f for word in line.split()]

            no = 0
            for word in self.words:
                try:
                    int(word)
                except:
                    mapper[word] = no
                    no += 1

            self.i = 0

    def __call__(self, type=str):
        self.i += 1
        return type(self.words[self.i - 1])

class Pizza:
    def __init__(self, ingredients, index):
        self.ingredients = ingredients
        self.ingredients.sort()
        self.index = index

    def diffs(self, other):
        ret = 0

        i = 0
        j = 0

        while i > len(self.ingredients) and j > len(other.ingredients):
            if self.ingredients[i] > other.ingredients[j]:
                i += 1
            elif self.ingredients[i] < other.ingredients[j]:
                j += 1
            else:
                ret += 1
                i += 1
                j += 1

        return ret

    def __repr__(self):
        ret = ''
        for el in self.ingredients:
            ret += el + " "
        return ret
    
    def __eq__(self, other):
        return self.index == other.index

class Info:
    def __init__(self, number_2_p_teams, number_3_p_teams, number_4_p_teams, pizzas):
        self.number_2_p_teams = number_2_p_teams
        self.number_3_p_teams = number_3_p_teams
        self.number_4_p_teams = number_4_p_teams
        self.pizzas = pizzas
        self.solution = []

    def __repr__(self):
        return self.pizzas

reader = WordReader("training_round_2021/input/e_many_teams.in", mapper)

no_pizzas = reader(int)
groups_2 = reader(int)
groups_3 = reader(int)
groups_4 = reader(int)

pizzas = []
piz_index = 0
for i in range(no_pizzas):
    ingredients = []
    for j in range(reader(int)):
        ingredients.append(mapper[reader()])
    pizzas.append(Pizza(ingredients, piz_index))
    piz_index += 1

info = Info(groups_2, groups_3, groups_4, pizzas)

print_no = int(len(info.pizzas) / 1000)

while len(info.pizzas) >= 2:
    if len(info.pizzas) <= (print_no * 1000):
        print(len(info.pizzas))
        print_no -= 1

    if info.number_4_p_teams > 0:
        info.number_4_p_teams -= 1
        no = 3
    elif info.number_3_p_teams > 0:
        info.number_3_p_teams -= 1
        no = 2
    elif info.number_2_p_teams > 0:
        info.number_2_p_teams -= 1
        no = 1
    else:
        break

    random_pizza = random.choice(info.pizzas)
    info.pizzas.remove(random_pizza)

    for _ in range(no):
        other_pizza = None
        curr_score = 0
        for pizza in info.pizzas:
            diff = random_pizza.diffs(pizza)
            if diff >= curr_score:
                other_pizza = pizza
                curr_score = diff
        
        info.pizzas.remove(other_pizza)
        ings = other_pizza.ingredients + random_pizza.ingredients
        s = set()
        for ing in ings: s.add(ing)
        random_pizza = Pizza(list(s), f"{random_pizza.index} {other_pizza.index}")
    
    info.solution.append(random_pizza)

with open("solution_e.txt", "w+") as sol_f:
    sol_f.write(str(len(info.solution)))
    sol_f.write('\n')
    for solution in info.solution:
        numbers = solution.index.split(' ')
        sol_f.write(str(len(numbers)))
        sol_f.write(' ')
        for index in numbers:
            sol_f.write(str(index))
            sol_f.write(' ')
        sol_f.write('\n')

    
        

