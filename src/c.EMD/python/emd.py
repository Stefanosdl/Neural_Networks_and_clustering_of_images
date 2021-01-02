import math
from collections import Counter
from collections import defaultdict
from ortools.linear_solver import pywraplp

def euclidean_distance(x, y):
    return math.sqrt((x - y)**2)

def EarthMoverDistance(p1, p2):
    dist1 = {x: float(count) / len(p1) for (x, count) in Counter(p1).items()}
    dist2 = {x: float(count) / len(p2) for (x, count) in Counter(p2).items()}
    variables = dict()
    
    solver = pywraplp.Solver('EarthMoverDistance', pywraplp.Solver.GLOP_LINEAR_PROGRAMMING)

    # for each pile in dist1, the constraint that says all the dirt must leave this pile
    dirt_leaving_constraints = defaultdict(lambda: 0)

    # for each hole in dist2, the constraint that says this hole must be filled
    dirt_filling_constraints = defaultdict(lambda: 0)

    # the objective
    objective = solver.Objective()
    objective.SetMinimization()

    for (x, dirt_at_x) in dist1.items():
        for (y, capacity_of_y) in dist2.items():
            amount_to_move_x_y = solver.NumVar(0, solver.infinity(), 'z_{%s, %s}' % (x, y))
            variables[(x, y)] = amount_to_move_x_y
            dirt_leaving_constraints[x] += amount_to_move_x_y
            dirt_filling_constraints[y] += amount_to_move_x_y
            objective.SetCoefficient(amount_to_move_x_y, euclidean_distance(x, y))

    for x, linear_combination in dirt_leaving_constraints.items():
        solver.Add(linear_combination == dist1[x])

    for y, linear_combination in dirt_filling_constraints.items():
        solver.Add(linear_combination == dist2[y])

    status = solver.Solve()
    if status not in [solver.OPTIMAL, solver.FEASIBLE]:
        raise Exception('Unable to find feasible solution')

    for ((x, y), variable) in variables.items():
        if variable.solution_value() != 0:
            cost = euclidean_distance(x, y) * variable.solution_value()
            # print("move {} dirt from {} to {} for a cost of {}".format(
                # variable.solution_value(), x, y, cost))

    return objective.Value()

if __name__ == "__main__":
    p1 = [0,1,3]
    p2 = [5,6,8]

    print(EarthMoverDistance(p1, p2))