import sys
from reader import handleInput, read_all_files
from brute import bruteForce
import time

def main(argv):
	
	p1 = [0,1,3]
	p2 = [5,6,8]

	# print(EarthMoverDistance(p1, p2))
	input_file_orifinal_space, query_file_orifinal_space, input_labels, query_labels, output_file = handleInput(argv)

	input_images, input_labels, query_images, query_labels, number_of_input_images, number_of_query_images, d_space = read_all_files(input_file_orifinal_space, input_labels, query_file_orifinal_space, query_labels)

	ofile = open(output_file, "w")

	trueTime = 0.0
	for q_num in range(0, number_of_query_images):
		start = time.time()
		n_neighbours = bruteForce(q_num, query_images, input_images, number_of_input_images, 10, d_space)
		end = time.time()
		# print(end-start)
		trueTime = trueTime + (end - start)
		# ofile.write()
	ofile.close()

if __name__ == "__main__":
	main(sys.argv[1:])
