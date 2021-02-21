from pyparser import file_to_parse
import numpy as np
import matplotlib.pyplot as plt



def scatter_score_v_assembly_points(file):
    file_info = file_to_parse(file)
    y = [len(task.assembly_points) for task in file_info.tasks]
    x = [task.score for task in file_info.tasks]



    plt.scatter(x, y)
    plt.title('score_v_assembly_points')
    plt.ylabel('Assembly Point Number')
    plt.xlabel('Score')
    plt.show()

def scatter_score_v_manhattand(file):
    file_info = file_to_parse(file)
    y = [task.total_distance for task in file_info.tasks]
    x = [task.score for task in file_info.tasks]


    plt.scatter(x, y)
    plt.title('score_v_manhattan_distance')
    plt.ylabel('Assembly Point Number')
    plt.xlabel('Score')
    plt.show()