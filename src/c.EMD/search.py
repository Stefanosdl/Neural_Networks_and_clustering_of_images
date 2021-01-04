import sys
import time
from python.reader import handleInput, read_all_files
from collections import OrderedDict
from python.emd import EarthMoverDistance

N = 2
sample = 2

def main(argv):
	input_file_orifinal_space, query_file_orifinal_space, input_labels, query_labels, output_file = handleInput(argv)
	input_images, input_labels, query_images, query_labels, number_of_input_images, number_of_query_images, d_space = read_all_files(input_file_orifinal_space, input_labels, query_file_orifinal_space, query_labels)
	ofile = open(output_file, "w")
	trueTime = 0.0
	# keep here the name of the N neighbours only, but for each q_num
	neighbours = []
	start = time.time()
	print("Starting...")
	for q_num in range(sample):
		n_neighbours = {}
		# number_of_input_images
		for p_num in range(number_of_input_images):
			dist = EarthMoverDistance(query_images[q_num], input_images[p_num])
			n_neighbours[p_num] = dist
		n_neighbours = OrderedDict(sorted(n_neighbours.items(), key=lambda t: t[1]))
		n_neighbours = list(n_neighbours.items())[:N]
		# now I have in n_neighbours
		# store them
		for i in range(N):
			neighbours.append(n_neighbours[i][0])

	print(neighbours)

	end = time.time()
	trueTime = end - start
	print(trueTime)
	ofile.close()

if __name__ == "__main__":
	main(sys.argv[1:])
