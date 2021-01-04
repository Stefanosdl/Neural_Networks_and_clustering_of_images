import sys
import time
from python.reader import handleInput, read_all_files
from python.emd import EarthMoverDistance

def main(argv):

	input_file_orifinal_space, query_file_orifinal_space, input_labels, query_labels, output_file = handleInput(argv)

	input_images, input_labels, query_images, query_labels, number_of_input_images, number_of_query_images, d_space = read_all_files(input_file_orifinal_space, input_labels, query_file_orifinal_space, query_labels)

	ofile = open(output_file, "w")

	trueTime = 0.0

	start = time.time()
	for q_num in range(10):
		for p_num in range (0, number_of_input_images):
			print(EarthMoverDistance(query_images[q_num], input_images[p_num]))
			# print(end-start)
			# ofile.write()
	end = time.time()
	trueTime = end - start
	print(trueTime)
	ofile.close()

if __name__ == "__main__":
	main(sys.argv[1:])
