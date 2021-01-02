from collections import OrderedDict

def manhattanDistance(q_array, p_array, d_space):
    result = 0
    for i in range(d_space):
        result = result + abs(q_array[i] - p_array[i])
    return result

def bruteForce(q_num, query_images, dataset_images, number_of_images, number_of_neighbours, d_space):
    # regular unsorted dict of neighbours
    n_neighbours = {}
    for i in range(number_of_images):
        dist = manhattanDistance(query_images[q_num], dataset_images[i], d_space)
        n_neighbours[i] = dist
    
    n_neighbours = OrderedDict(sorted(n_neighbours.items(), key=lambda t: t[1]))
    n_neighbours = list(n_neighbours.items())[:number_of_neighbours]
    return n_neighbours
