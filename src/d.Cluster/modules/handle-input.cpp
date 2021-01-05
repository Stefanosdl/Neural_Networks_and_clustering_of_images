// Function to handle the input given from command line
#include <iostream>
#include <sstream>
#include <cmath>
#include "../headers/common.hpp"
#include "../headers/handle-input.hpp"
using namespace std;

int** all_images_original_space = NULL;
int** all_images_new_space = NULL;

void handleReExecution (
	uint32_t *number_of_images, 
	uint64_t *d_original,
	int *K_medians,
	int *k,
	int *l,
	string *output_file
	) {
	string input_file_original_space, input_file_new_space, clusters_file, conf_file, param, input;
	cout << "Please enter new parameters for the program to re-execute or press enter to exit" << endl;
	char split_char = ' ';
	getline(cin, input);
	if (input.size() == 0)
		exit(SUCCESS);
	// Here the least amount of arguments are 10
	// We don't pass the executable name as parameter
	if (input.size() < 10) {
		cout << "You need to provide the path of the files" << endl;
		exit(ERROR);
	}

	// stringstream class token
	stringstream token(input);
	// tokenize the input and store the values
	while (getline(token, param, split_char)) {
		if (param == "-i") {
			getline(token, param, split_char);
			input_file_new_space = param;
		}
		else if (param == "-n") {
			getline(token, param, split_char);
			clusters_file = param;
		}
		else if (param == "-o")	{
			getline(token, param, split_char);
			*output_file = param;
		}
		else if (param == "-d") {
			getline(token, param, split_char);
			input_file_original_space = param;
		}
		else if (param == "-c")	{
			getline(token, param, split_char);
			conf_file = param;
		}
	}
	// Check if the files are provided
	if (input_file_original_space.empty())	{
		cout << "You need to provide the input_file_original_space path" << endl;
		exit(ERROR);
	}
	if (input_file_new_space.empty())	{
		cout << "You need to provide the input_file_new_space path" << endl;
		exit(ERROR);
	}
	if (clusters_file.empty())	{
		cout << "You need to provide the clusters_file path" << endl;
		exit(ERROR);
	}
	if (output_file->empty()) {
		cout << "You need to provide the output_file path" << endl;
		exit(ERROR);
	}
	if (conf_file.empty()) {
		cout << "You need to provide the conf_file path" << endl;
		exit(ERROR);
	}
	readFileOriginalSpace(input_file_original_space, INPUT_FILE, number_of_images, d_original);
	readConfFile(conf_file, K_medians, l, k);
}

void handleInput(
	int argc,
	char **argv,
	uint32_t *number_of_images, 
	uint64_t *d_original,
	int *K_medians,
	int *k,
	int *l,
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
	readConfFile(conf_file, K_medians, l, k);
	// returning these values to main to continue execution
}