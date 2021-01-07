// Function to handle the input given from command line
#include <iostream>
#include <sstream>
#include <cmath>
#include "../headers/common.hpp"
#include "../headers/handle-input.hpp"
using namespace std;

int** all_images_original_space = NULL;
int** all_images_new_space = NULL;

void handleInput(
	int argc,
	char **argv,
	uint32_t *number_of_images, 
	uint64_t *d_original,
	int *K_medians,
	string *output_file
	){
	string input_file_original_space, input_file_new_space, clusters_file, conf_file, param;
	// First we need to check for the least amount of arguments required
	// Which are 11 since we need 4 files with their param and the executable
	if (argc < 11) {
		cerr << "You need to provide the path of the files" << endl;
		exit(ERROR);
	}
	param = argv[1];
	// get the path files
	if (param != "-d") {
		cerr << "You need to provide the input_file_original_space path" << endl;
		exit(ERROR);
	}
	input_file_original_space = argv[2];

	param = argv[3];
	if (param != "-i") {
		cerr << "You need to provide the input_file_new_space path" << endl;
		exit(ERROR);
	}
	*input_file_new_space = argv[4];

 	param = argv[5];
	if (param != "-n") {
		cerr << "You need to provide the clusters_file path" << endl;
		exit(ERROR);
	}
	clusters_file = argv[6];

	param = argv[7];
	if (param != "-c") {
		cerr << "You need to provide the configuration_file path" << endl;
		exit(ERROR);
	}
	conf_file = argv[8];

	param = argv[9];
	if (param != "-o") {
		cerr << "You need to provide the output_file path" << endl;
		exit(ERROR);
	}
	*output_file = argv[10];

	// read data from the input_file
	readFileOriginalSpace(input_file_original_space, INPUT_FILE, number_of_images, d_original);
	readConfFile(conf_file, K_medians);
	// returning these values to main to continue execution
}