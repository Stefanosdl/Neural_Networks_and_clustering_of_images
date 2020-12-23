// Function to handle the input given from command line
#include <iostream>
#include <sstream>
#include <cmath>
#include "../headers/common.hpp"
#include "../headers/handle-input.hpp"
#include "../headers/manhattan-hashing.hpp"
#include "../headers/hashtable.hpp"
using namespace std;

unsigned int M;
unsigned int m;
int** all_images = NULL;
int** query_images = NULL;
unsigned int w;

void handleReExecution (
	uint32_t *number_of_images, 
	uint64_t *d,
	int *k,
	int *l,
	string *output_file,
	string *query_file_original_space,
	string *query_file_new_space
	) {
	string input_file_original_space, input_file_new_space, param, input;
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
		if (param == "-k") {
			getline(token, param, split_char);
			stringstream intValue(param);
			intValue >> *k;
		}
		else if (param == "-L") {
			getline(token, param, split_char);
			stringstream intValue(param);
			intValue >> *l;
		}
		else if (param == "-o")	{
			getline(token, param, split_char);
			*output_file = param;
		}
		else if (param == "-d") {
			getline(token, param, split_char);
			input_file_original_space = param;
		}
		else if (param == "-i") {
			getline(token, param, split_char);
			input_file_new_space = param;
		}
		else if (param == "-q")	{
			getline(token, param, split_char);
			*query_file_original_space = param;
		}
		else if (param == "-s")	{
			getline(token, param, split_char);
			*query_file_new_space = param;
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
	if (output_file->empty()) {
		cout << "You need to provide the output_file path" << endl;
		exit(ERROR);
	}
	if (query_file_original_space->empty()) {
		cout << "You need to provide the query_file_original_space path" << endl;
		exit(ERROR);
	}
	if (query_file_new_space->empty()) {
		cout << "You need to provide the query_file_new_space path" << endl;
		exit(ERROR);
	}

	if (*k < 2)	{
		cerr << "The number k should be more than 2, from theory, ideally should be 4" << endl;
		exit(ERROR);
	}
	// M as given from theory
	M = pow(2, 32/(*k));
	m = M/2 - 1;
	readFile(input_file, INPUT_FILE, number_of_images, d, *k, *l);
}

void handleInput(
	int argc, 
	char **argv, 
	uint32_t *number_of_images, 
	uint64_t *d,
	int *k,
	int *l,
	string *output_file,
	string *query_file_original_space,
	string *query_file_new_space
	){
	string input_file_original_space, input_file_new_space, param;
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
	input_file_new_space = argv[4];

 	param = argv[5];
	if (param != "-q") {
		cerr << "You need to provide the query_file_original_space path" << endl;
		exit(ERROR);
	}
	*query_file_original_space = argv[6];

	param = argv[7];
	if (param != "-s") {
		cerr << "You need to provide the query_file_new_space path" << endl;
		exit(ERROR);
	}
	*query_file_new_space = argv[8];

	for (int i = 9; i < argc; i++) {
		param = argv[i];
		if (!argv[i+1]) exit(ERROR);
		if (param == "-k") *k = atoi(argv[++i]);
		else if (param == "-L") *l = atoi(argv[++i]);
		else if (param == "-o") *output_file = argv[++i];
	}

	if (output_file->empty()) {
		cerr << "You need to provide the output_file path" << endl;
		exit(ERROR);
	}
	
	if (*k < 2) {
		cerr << "The number k should be more than 2, from theory, ideally should be 4" << endl;
		exit(ERROR);
	}
	// M as given from theory
	M = pow(2, 32/(*k));
	m = M/2 - 1;
	// read data from the input_file 
	readFile(input_file, INPUT_FILE, number_of_images, d, *k, *l);
	// returning these values to main to continue execution
}