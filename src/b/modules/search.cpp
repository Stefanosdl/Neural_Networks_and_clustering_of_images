#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "../headers/common.hpp"
#include "../headers/manhattan-hashing.hpp"
#include "../headers/distances.hpp"
#include "../headers/handle-input.hpp"
#include "../headers/modulo.hpp"
#include "../headers/hashtable.hpp"
using namespace std;
// times to be written in the end of the output file
chrono::duration<double> elapsedLSH;
chrono::duration<double> elapsedTrueNewSpace;
chrono::duration<double> elapsedTrueOriginalSpace;
// structures to keep data for calculation of the Approximation Factor
vector <pair<int, unsigned int> > bruteNewSpaceNeighbours;
vector <pair<int, unsigned int> > LSHNeighbours;
vector <pair<int, unsigned int> > trueNeighbours;
//
void calculateApproximationFactor(int number_of_query_images, int d_original, double& approxFactorLSH, double& approxFactorNewSpace) {
    // double approxFactorLSH = 0.0, approxFactorNewSpace = 0.0;
    for (int i = 0; i < number_of_query_images; i++) {
        // for LSH
        if (LSHNeighbours[i].second != 0) {
            approxFactorLSH += static_cast<double>(LSHNeighbours[i].second) / static_cast<double>(trueNeighbours[i].second);
        }
        // for New space 
        unsigned int new_space_dist = manhattanDistance(query_images_original_space[i], all_images_original_space[bruteNewSpaceNeighbours[i].first], d_original);
        approxFactorNewSpace += static_cast<double>(new_space_dist) / static_cast<double>(trueNeighbours[i].second);
    }

    approxFactorLSH /= static_cast<double>(number_of_query_images);
    approxFactorNewSpace /= static_cast<double>(number_of_query_images);
}

// Brute Force
pair <unsigned int, unsigned int> approximateN_NN_Full_Search(uint64_t d, uint32_t q_num, int number_of_images, int** new_query, int** dataset) {
    pair <unsigned int, unsigned int> n_neighbour;
    n_neighbour = make_pair(0, 4294967295);
    unsigned int current_distance = 0;
    // loop over the images array
    for (int i=0; i<number_of_images; i++) {
        current_distance = manhattanDistance(new_query[q_num], dataset[i], d);
        if (current_distance < n_neighbour.second) {
            n_neighbour = make_pair(i, current_distance);
        }
    }
    return n_neighbour;
}
// two ways to find this
// A. LSH
// B. True (brute force)
// takes a N from user 
// calls approximateNN with the correct way (A or B)
// q_num: is an index of the query to search in the query_images[]
void approximateN_NNs (ofstream* file, uint64_t d_original, uint64_t d, int k, int L, uint32_t q_num, int number_of_images) {
    pair <unsigned int, unsigned int> n_neighbours;
    unsigned int min_distance = inf;
    unsigned int current_gp = 0, current_gq = 0;
    unsigned int current_distance = 0;
    int pos_in_hash = 0;
    pair<unsigned int, unsigned int> BNNNewSpace;
    pair<unsigned int, unsigned int> BNNOriginalSpace;
    int hashtable_size = number_of_images / HASHTABLE_NUMBER;
    // we start putting neighbours from farthest to closest
    // calculating g(q)
    current_gq = calculateG_X(k, d_original, q_num, QUERY_FILE);
    pos_in_hash = customModulo(current_gq, hashtable_size);
    if (pos_in_hash > hashtable_size - 1) {
        // then something went wrong with g(p)
        cerr << "Calculating g(q) went wrong" << endl;
        exit(ERROR);
    }
    auto startLSH = chrono::high_resolution_clock::now();
    // for all hash_tables
    for (int l = 0; l < L; l++) {
        // loop over the bucket
        for (unsigned int h = 0; h < HashTables[l][pos_in_hash].size(); h++) {
            // calculate the Manhattan distance of q and every other image in the bucket
            // distance between HashTables[l][pos_in_hash][h], and query_image[q_num]
            // calculate distance and g_p
            current_distance = manhattanDistance(query_images_original_space[q_num], all_images_original_space[HashTables[l][pos_in_hash][h].first], d_original);
            current_gp = HashTables[l][pos_in_hash][h].second;
            // delete the parray now
            if (current_distance < min_distance && current_gq == current_gp) {
                n_neighbours = make_pair(HashTables[l][pos_in_hash][h].first, current_distance);
                min_distance = current_distance;
            }
            // take into account a maximum of (10 * L) points in each hashtable
            if (h > static_cast<unsigned int>(10*L)) {
                // break the loop to continue with next hashtable
                break;
            }
        }
        // finished with this hash table
    }
    
    auto finishLSH = chrono::high_resolution_clock::now();
    elapsedLSH += finishLSH - startLSH;

    // Do Brute for new Space

    auto startTrueNewSpace = chrono::high_resolution_clock::now();
    BNNNewSpace = approximateN_NN_Full_Search(d, q_num, number_of_images, query_images, all_images);
    auto finishTrueNewSpace = chrono::high_resolution_clock::now();
    elapsedTrueNewSpace += finishTrueNewSpace - startTrueNewSpace;

    // Do Brute for original Space

    auto startTrueOriginalSpace = chrono::high_resolution_clock::now();
    BNNOriginalSpace = approximateN_NN_Full_Search(d_original, q_num, number_of_images, query_images_original_space, all_images_original_space);
    auto finishTrueOriginalSpace = chrono::high_resolution_clock::now();
    elapsedTrueOriginalSpace += finishTrueOriginalSpace - startTrueOriginalSpace;
    
    // finish all brute forces write to file
    (*file) << "Query: " << q_num << endl;
    (*file) << "Nearest neighbor Reduced:" << BNNNewSpace.first << endl;
    (*file) << "Nearest neighbor LSH:" << n_neighbours.first << endl;
    (*file) << "Nearest neighbor True:" << BNNOriginalSpace.first << endl;
    
    (*file) << "distanceReduced: " << BNNNewSpace.second << endl;
    (*file) << "distanceLSH: " << n_neighbours.second << endl;
    (*file) << "distanceTrue: " << BNNOriginalSpace.second << endl;

    // push back to the global structs the findings for this image
    bruteNewSpaceNeighbours.push_back(BNNNewSpace);
    LSHNeighbours.push_back(n_neighbours);
    trueNeighbours.push_back(BNNOriginalSpace);
}

void writeLastMeta(ofstream* file, int number_of_query_images, int d_original) {
    double approxFactorLSH = 0.0, approxFactorNewSpace = 0.0;
    calculateApproximationFactor(number_of_query_images, d_original, approxFactorLSH, approxFactorNewSpace);
    (*file) << endl << "tReduced: " << elapsedTrueNewSpace.count() << endl;
    (*file) << "tLSH: " << elapsedLSH.count() << endl;
    (*file) << "tTrue: " << elapsedTrueOriginalSpace.count() << endl;
    
    (*file) << "Approximation Factor LSH: " << approxFactorLSH << endl;
    (*file) << "Approximation Factor Reduced: " << approxFactorNewSpace << endl;
}