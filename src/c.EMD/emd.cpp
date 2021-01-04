#include <iostream>
#include <chrono>
#include "headers/common.hpp"
#include "headers/search.hpp"
#include "headers/handle-input.hpp"
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

int main(int argc, char **argv) {
    uint32_t number_of_images = 0;
    uint32_t number_of_query_images = 0;
    uint64_t d_original = 0;
    string output_file, query_file_original_space, labels_query;
    ofstream o_file;
    unordered_map<int , vector<int> > Brute;
    chrono::duration<double> elapsedBruteTimer;

    handleInput(argc, argv, &number_of_images, &d_original, &output_file, &query_file_original_space, &labels_query);
    // open output file
    o_file.open(output_file);
    if (!o_file.is_open()) {
        cerr << "Output file can't be opened" << endl;
        exit(ERROR);
    }
    do {
        readFileOriginalSpace(query_file_original_space, QUERY_FILE, &number_of_query_images, &d_original);
        readFileOriginalSpace(labels_query, QUERY_LABELS, &number_of_query_images, &d_original);

        auto startBruteTimer = chrono::high_resolution_clock::now();
        for (uint32_t q_num = 0; q_num < SAMPLE; q_num++) {
            // Brute
            Brute_Force(d_original, q_num, number_of_images, Brute);
        }
        auto finishBruteTimer = chrono::high_resolution_clock::now();
        elapsedBruteTimer += finishBruteTimer - startBruteTimer;
        cout << "Brute Timer is: " << elapsedBruteTimer.count() << endl;

        Evaluate_Results(SAMPLE, Brute, &o_file);

        // Call python file to run EMD algorithm
        // string params = ("/usr/bin/python3 ./search.py ");
        // for (int i=1; i<argc ; i++) {
        //     params += argv[i];
        //     params += " ";
        // }
        // int result = system(params.c_str());

        o_file.close();
        
        handleReExecution(&number_of_images, &d_original, &output_file, &query_file_original_space, &labels_query);
        // open output file
        o_file.open(output_file);
        if (!o_file.is_open()){
            cerr << "Output file can't be opened" << endl;
            exit(ERROR);
        }
    } while (true);
    o_file.close();
    
    // Free spaces
    for (uint32_t i = 0; i < number_of_images;i++) {
        delete[] all_images_original_space[i];
    }
    for (uint32_t i = 0; i < number_of_query_images;i++) {
        delete[] query_images_original_space[i];
    }
    delete[] all_images_original_space;
    delete[] query_images_original_space;
    delete[] all_images_labels;
    delete[] query_images_labels;


    
    return SUCCESS;
}
