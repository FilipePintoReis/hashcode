
class Information:
    def __init__(self, width, height, no_arm, mount_points, tasks, steps):
        self.width = width
        self.height = height
        self.no_arm = no_arm
        self.mount_points = mount_points
        self.tasks = tasks
        self.steps = steps
    
    def __repr__(self):
        ret = f'width {self.width}, height {self.height}, no_arm {self.no_arm}, steps {self.steps}\n mount_points {self.mount_points},\n tasks {self.tasks}'
        return ret

class Task:
    def __init__(self, score, assembly_points):
        self.score = score
        self.assembly_points = assembly_points
        self.total_distance = 0

        for i in range(len(assembly_points) - 1):
            x1, y1 = assembly_points[i]
            x2, y2 = assembly_points[i + 1]
            self.total_distance += abs(x1 - x2) + abs(y1 - y2)

    def __repr__(self):
        ret = f'\nscore = {self.score}, assembly_points = {self.assembly_points}'
        return ret

def file_to_parse(file):
    with open(f'input/{file}', 'r') as f:
        lines = f.readlines()
        for i in range(len(lines)):
            lines[i] = lines[i].replace('\n','')
        lines[0] = lines[0].split(' ')
        width = int(lines[0][0])
        height = int(lines[0][1])
        no_arms = int(lines[0][2])
        no_mount_points = int(lines[0][3])
        steps = int(lines[0][5])

        mount_point_locations = []
        for el in lines[1:no_mount_points]:
            x, y = el.split(' ')
            mount_point_locations.append([int(x),int(y)])

        tasks = []
        flag = False
        for i in range(no_mount_points+1, len(lines)):
            if not flag:
                flag = True
                line = lines[i].split(' ')
                score = int(line[0])

            elif flag:
                flag = False
                line = lines[i].split(' ')
                assembly_points = []
                for k in range(0, len(line), 2):
                    assembly_points.append([int(line[k]), int(line[k+1])])

                tasks.append(Task(score, assembly_points))

        return Information(width, height, no_arms, mount_point_locations, tasks, steps)


