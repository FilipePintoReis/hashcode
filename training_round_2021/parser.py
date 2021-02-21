
class Pizza:
    def __init__(self, ingredients):
        self.ingredients = ingredients

    def __repr__(self):
        return self.ingredients

class Info:
    def __init__(self, number_2_p_teams, number_3_p_teams, number_4_p_teams, pizzas):
        self.number_2_p_teams = number_2_p_teams
        self.number_3_p_teams = number_3_p_teams
        self.number_4_p_teams = number_4_p_teams
        self.pizzas = pizzas

    def __repr__(self):
        return self.pizzas

def func(file):
    with open(file, "r") as f:
        lines = f.readlines()
        concat_lines = [line for line in lines]

        number_2_p_teams = concat_lines[1]
        number_3_p_teams = concat_lines[2]
        number_4_p_teams = concat_lines[3]
        
        it = 4
        list_of_pizzas = []
        while(it <= len(concat_lines) - 1):
            ingredients = []
            for i in range(concat_lines[it]):
                ingredients.append(concat_lines[it + 1 + i])

            list_of_pizzas.append(Pizza(ingredients))
            
            it += concat_lines[it] + 1

        info = Info(number_2_p_teams, number_3_p_teams,number_4_p_teams, list_of_pizzas)

        return info



