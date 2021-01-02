import getopt, sys

def handleInput(argv):
	input_file_orifinal_space = ''
	query_file_orifinal_space = ''
	input_labels = ''
	query_labels = ''
	output_file = ''
	emd = ''

	for i in range(0, len(argv)):
		if argv[i] == "-d":
			try:
				input_file_orifinal_space = argv[i+1]
			except IndexError:
				sys.exit("You need to provide a input_file_orifinal_space file path")
		elif argv[i] == "-q":
			try:
				query_file_orifinal_space = argv[i+1]
			except IndexError:
				sys.exit("You need to provide a query_file_orifinal_space file path")
		elif argv[i] == "-l1":
			try:
				input_labels = argv[i+1]
			except IndexError:
				sys.exit("You need to provide a input_labels file path")
		elif argv[i] == "-l2":
			try:
				query_labels = argv[i+1]
			except IndexError:
				sys.exit("You need to provide a query_labels file path")
		elif argv[i] == "-o":
			try:
				output_file = argv[i+1]
			except IndexError:
				sys.exit("You need to provide a output_file file path")
		elif argv[i] == "-EMD":
			try:
				emd = argv[i]
			except IndexError:
				sys.exit("You need to provide the emd parameter")
	
	if not emd:
		sys.exit("You need to provide the emd parameter")
	elif not input_file_orifinal_space:
		sys.exit("You need to provide a input_file_orifinal_space file path")
	elif not query_file_orifinal_space:
		sys.exit("You need to provide a query_file_orifinal_space file path")
	elif not input_labels:
		sys.exit("You need to provide a input_labels file path")
	elif not query_labels:
		sys.exit("You need to provide a query_labels file path")
	return (input_file_orifinal_space, query_file_orifinal_space, input_labels, query_labels, output_file)

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

def reader_labels(dataset):
	# create a dictionary of lists to store all images
	all_labels = []
	with open(dataset, "rb") as f:
		counter = 0
		# read metadata
		while (byte:= f.read(4)):
			if counter == 0:
				magic_number = int.from_bytes(byte, "big")
			elif counter == 1:
				number_of_items = int.from_bytes(byte, "big")
				break
			counter += 1
		# start reading the labels 
		while (byte:= f.read(1)):
			# store byte in the 
			all_labels.append(int.from_bytes(byte, "big"))
	# finished with reading of file
	return all_labels


def read_all_files(training_set, training_labels, test_set, test_labels):
	train_images = reader(training_set)
	train_labels = reader_labels(training_labels) 
	test_images = reader(test_set)
	test_labels = reader_labels(test_labels)
	# labels are lists of 10000 items
	return (train_images, train_labels, test_images, test_labels)