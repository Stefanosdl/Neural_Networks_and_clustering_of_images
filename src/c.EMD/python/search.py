import sys
from reader import handleInput, read_all_files

def main(argv):
	
	p1 = [0,1,3]
	p2 = [5,6,8]

	# print(EarthMoverDistance(p1, p2))
	input_file_orifinal_space, query_file_orifinal_space, input_labels, query_labels, output_file = handleInput(argv)

	input_images, input_labels, query_images, query_labels = read_all_files(input_file_orifinal_space, input_labels, query_file_orifinal_space, query_labels)

if __name__ == "__main__":
	main(sys.argv[1:])
