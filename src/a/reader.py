import getopt, sys
import struct
import numpy as np

def handleInput(argv):
	dataset = ''
	queryset = ''
	output_dataset_file = ''
	output_query_file = ''

	for i in range(0, len(argv)):
		if argv[i] == "-d":
			try:
				dataset = argv[i+1]
			except IndexError:
				sys.exit("You need to provide a dataset file path")
		elif argv[i] == "-q":
			try:
				queryset = argv[i+1]
			except IndexError:
				sys.exit("You need to provide a queryset file path")
		elif argv[i] == "-od":
			try:
				output_dataset_file = argv[i+1]
			except IndexError:
				sys.exit("You need to provide a output_dataset_file file path")
		elif argv[i] == "-oq":
			try:
				output_query_file = argv[i+1]
			except IndexError:
				sys.exit("You need to provide a output_query_file file path")
	return (dataset, queryset, output_dataset_file, output_query_file)

def reader(dataset):
	# create a dictionary of lists to store all images
	all_images = {}
	all_images[0] = []
	with open(dataset, "rb") as f:
		counter = 0
		# read metadata
		while (byte:= f.read(4)):
			if counter == 0:
				magic_number = int.from_bytes(byte, "big")
			elif counter == 1:
				number_of_images = int.from_bytes(byte, "big")
			elif counter == 2:
				rows = int.from_bytes(byte, "big")
			elif counter == 3:
				cols = int.from_bytes(byte, "big")
				break
			counter += 1
		# start reading the images 
		byte_counter = 0
		image_counter = 0
		dimensions = rows * cols
		while (byte:= f.read(1)):
			# store byte in the 
			all_images[image_counter].append(int.from_bytes(byte, "big"))
			byte_counter += 1
			if (byte_counter == dimensions):
				# next image
				image_counter += 1
				byte_counter = 0
				# initialize the list for this image
				all_images[image_counter] = []
	# finished with reading of file
	# remove last item number_of_images index that is anyway an empty list
	all_images.popitem()
	# return all_images dict
	return all_images

def read_all_files(training_set, test_set):
	train_images = reader(training_set)
	test_images = reader(test_set)
	return (train_images, test_images)

def writeToFile(normalized, path):
	normalized.astype(np.ushort)
	outputfile = open(path, "wb")
	number_of_images = normalized.shape[0]
	dimensions = normalized.shape[1]
	# write metadata
	# enforce big endian
	myfmt = '>i'
	# write magic number
	bin = struct.pack(myfmt, 2081)
	outputfile.write(bin)
	# write num of images
	bin = struct.pack(myfmt, number_of_images)
	outputfile.write(bin)
	# write num of rows (1)
	bin = struct.pack(myfmt, 1)
	outputfile.write(bin)
	# write dimensions
	bin = struct.pack(myfmt, dimensions)
	outputfile.write(bin)
	# enforce little endian
	myfmt = 'H'*dimensions
	myfmt = '<' + myfmt
	#  You can use 'H' for unsigned short and <, for little endian to force endinness
	for i in range(0, number_of_images):
		arr = np.array(normalized[i])
		bin = struct.pack(myfmt, *arr)
		outputfile.write(bin)

	outputfile.close()