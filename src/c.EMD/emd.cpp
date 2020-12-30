#include <iostream>
#include "headers/common.hpp"
#include "headers/handle-input.hpp"
#include "headers/wasserstein.hpp"
using namespace std;
double EMDdistance(int* query_image, size_t size_query_image, int* test_image, size_t size_test_image);

int main(int argc, char **argv) {
    
    uint32_t number_of_images = 0;
    uint32_t number_of_query_images = 0;
    uint64_t d_original = 0;
    string output_file, query_file_original_space, labels_query;
    ofstream o_file;

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
        
        // printFiles(number_of_images, number_of_query_images, d_original);
        for (uint32_t q_num = 0; q_num < number_of_query_images; q_num++) {
            // Brute
            // EMD
        }
        // writeLastMeta(&o_file, number_of_query_images, d_original);
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
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    // int query_image[] = {0,1,3};
    // int test_image[] = {5,6,8};
    // // ............... Call EMD
    // size_t size_q = sizeof query_image;
    // size_t size_t = sizeof test_image;
    // cout << EMDdistance(query_image, size_q, test_image, size_t) << endl;
}

double EMDdistance(int* query_image, size_t size_query_image, int* test_image, size_t size_test_image) {
    vector<int> av(query_image, query_image + size_query_image / sizeof query_image[0]);
    vector<int> bv(test_image, test_image + size_test_image / sizeof test_image[0]);
    vector<int> weights;

    for (unsigned int i = 0; i < av.size(); i++){
        weights.push_back(1);
    }

    double dist = wasserstein(av, weights, bv, weights);
    av.clear();
    bv.clear();
    weights.clear();
    return dist;
}