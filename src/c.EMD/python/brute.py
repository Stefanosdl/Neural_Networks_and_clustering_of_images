from collections import OrderedDict
import time
import math
import operator

def manhattanDistance(q_array, p_array, d_space):
    # for i in range(d_space):
    #     result = result + abs(q_array[i] - p_array[i])

    # result = sum(abs(q - p) for (q,p) in zip(q_array, p_array))

    # result = sum(map(int.__sub__, q_array, p_array))

    result = sum(map(abs, map(operator.sub, q_array, p_array)))
    

    return result

def bruteForce(q_num, query_images, dataset_images, number_of_images, number_of_neighbours, d_space):
    # regular unsorted dict of neighbours
    n_neighbours = {}
    d = 0
    man = 0
    for i in range(number_of_images):


        start = time.time()
        dist = manhattanDistance(query_images[q_num], dataset_images[i], d_space)
        end = time.time()
        man = man + (end-start)


        start = time.time()

        n_neighbours[i] = dist
        end = time.time()
        d = d + (end-start)


    # print ("MAN   " , man)
    # print("ANATHESI   ",d)

    start = time.time()
    n_neighbours = OrderedDict(sorted(n_neighbours.items(), key=lambda t: t[1]))
    n_neighbours = list(n_neighbours.items())[:number_of_neighbours]
    end = time.time()

    # print("Dictionary operaations", end-start)

    return n_neighbours
