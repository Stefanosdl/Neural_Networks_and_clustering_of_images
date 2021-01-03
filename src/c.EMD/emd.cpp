#include <iostream>
#include "headers/common.hpp"
#include "headers/search.hpp"
#include "headers/handle-input.hpp"
#include <vector>
#include <set>
using namespace std;

void Print_Vector(vector<int> Vec) {
    for (unsigned int i = 0; i < Vec.size(); i++) {
        cout << Vec[i] << " ";
    }
    cout << endl;
    return;
}

int main(int argc, char **argv) {
    uint32_t number_of_images = 0;
    uint32_t number_of_query_images = 0;
    uint64_t d_original = 0;
    string output_file, query_file_original_space, labels_query;
    ofstream o_file;
    set<vector<int> > Brute;

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
            Brute_Force(d_original, q_num, number_of_images, Brute);
            // EMD
        }
        cout<<"TELOS BRUTE"<<endl;
        for (auto it = Brute.begin(); it != Brute.end(); it++) { 
            Print_Vector(*it); 
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
