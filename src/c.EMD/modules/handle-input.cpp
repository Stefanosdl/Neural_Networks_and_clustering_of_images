// Function to handle the input given from command line
#include <iostream>
#include <sstream>
#include <cmath>
#include "../headers/common.hpp"
#include "../headers/handle-input.hpp"
using namespace std;

int** all_images_original_space = NULL;
int** query_images_original_space = NULL;
int* all_images_labels = NULL;
int* query_images_labels = NULL;

void handleReExecution (
	uint32_t *number_of_images, 
	uint64_t *d_original,
	string *output_file,
	string *query_file_original_space,
	string *labels_query
	) {
	string input_file_original_space, labels_input, param, input, emd;
	cout << "Please enter new parameters for the program to re-execute or press enter to exit" << endl;
	char split_char = ' ';
	getline(cin, input);
	if (input.size() == 0)
		exit(SUCCESS);
	// Here the least amount of arguments are 10
	// We don't pass the executable name as parameter
	if (input.size() < 11) {
		cout << "You need to provide the path of the files" << endl;
		exit(ERROR);
	}

	// stringstream class token
	stringstream token(input);
	// tokenize the input and store the values
	while (getline(token, param, split_char)) {
		if (param == "-l1") {
			getline(token, param, split_char);
			labels_input = param;
		}
		else if (param == "-l2") {
			getline(token, param, split_char);
			*labels_query = param;
		}
		else if (param == "-o")	{
			getline(token, param, split_char);
			*output_file = param;
		}
		else if (param == "-d") {
			getline(token, param, split_char);
			input_file_original_space = param;
		}
		else if (param == "-q")	{
			getline(token, param, split_char);
			*query_file_original_space = param;
		}
		else if (param == "-EMD")	{
			getline(token, param, split_char);
			emd = param;
		}
	}
	// Check if the files are provided
	if (input_file_original_space.empty())	{
		cout << "You need to provide the input_file_original_space path" << endl;
		exit(ERROR);
	}
	if (labels_input.empty())	{
		cout << "You need to provide the labels_of_input_dataset path" << endl;
		exit(ERROR);
	}
	if (labels_query->empty())	{
		cout << "You need to provide the labels_of_query_dataset path" << endl;
		exit(ERROR);
	}
	if (output_file->empty()) {
		cout << "You need to provide the output_file path" << endl;
		exit(ERROR);
	}
	if (query_file_original_space->empty()) {
		cout << "You need to provide the query_file_original_space path" << endl;
		exit(ERROR);
	}
	if (emd.empty()) {
		cerr << "You need to provide the EMD parameter" << endl;
		exit(ERROR);
	}

	readFileOriginalSpace(input_file_original_space, INPUT_FILE, number_of_images, d_original);
	readFileOriginalSpace(labels_input, INPUT_LABELS, number_of_images, d_original);
}

void handleInput(
	int argc, 
	char **argv, 
	uint32_t *number_of_images, 
	uint64_t *d_original,
	string *output_file,
	string *query_file_original_space,
	string *labels_query
	){
	string input_file_original_space, labels_input, param;
	// First we need to check for the least amount of arguments required
	// Which are 11 since we need 4 files with their param and the executable
	if (argc < 12) {
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
	if (param != "-q") {
		cerr << "You need to provide the query_file_original_space path" << endl;
		exit(ERROR);
	}
	*query_file_original_space = argv[4];

 	param = argv[5];
	if (param != "-l1") {
		cerr << "You need to provide the labels_of_input_dataset path" << endl;
		exit(ERROR);
	}
	labels_input = argv[6];

	param = argv[7];
	if (param != "-l2") {
		cerr << "You need to provide the labels_of_query_dataset path" << endl;
		exit(ERROR);
	}
	*labels_query = argv[8];

	param = argv[9];
	if (param != "-o") {
		cerr << "You need to provide the output_file path" << endl;
		exit(ERROR);
	}
	*output_file = argv[10];

	param = argv[11];
	if (param != "-EMD") {
		cerr << "You need to provide the EMD parameter" << endl;
		exit(ERROR);
	}

	// read data from the input_file
	readFileOriginalSpace(input_file_original_space, INPUT_FILE, number_of_images, d_original);
	readFileOriginalSpace(labels_input, INPUT_LABELS, number_of_images, d_original);
	// returning these values to main to continue execution
}