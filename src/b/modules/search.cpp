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

// Brute Force
vector<pair <unsigned int, unsigned int> > approximateN_NNs_Full_Search(uint64_t d, int n, uint32_t q_num, int number_of_images) {
    vector<pair <unsigned int, unsigned int> > n_neighbours;
    vector<pair <unsigned int, unsigned int> >::iterator it;
    unsigned int current_distance = 0;
    int* qarray, *parray;
    qarray = convertArray(query_images[q_num], d);

    // loop over the images array
    for (int i=0; i<number_of_images; i++) {
        parray = convertArray(all_images[i], d);
        current_distance = manhattanDistance(qarray, parray, d);
        delete[] parray; 
        n_neighbours.push_back(make_pair(i, current_distance));
    }
    delete[] qarray;
    sort(n_neighbours.begin(), n_neighbours.end(), [](const pair<unsigned int, unsigned int> &left, const pair<unsigned int, unsigned int> &right) {
        return left.second < right.second;
    });
    if (n_neighbours.size() >  static_cast<unsigned int>(n)) n_neighbours.resize(n);
    return n_neighbours;
}
// two ways to find this
// A. LSH
// B. True (brute force)
// takes a N from user 
// calls approximateNN with the correct way (A or B)
// q_num: is an index of the query to search in the query_images[]
void approximateN_NNs (ofstream* file, uint64_t d, int k, int n, int L, uint32_t q_num, int number_of_images) {
    vector<pair <unsigned int, unsigned int> > n_neighbours;
    unsigned int min_distance = inf;
    unsigned int current_gp = 0, current_gq = 0;
    unsigned int current_distance = 0;
    int pos_in_hash = 0;
    vector<pair<unsigned int, unsigned int> > BNN;
    int hashtable_size = number_of_images / HASHTABLE_NUMBER;
    // we start putting neighbours from farthest to closest
    int* qarray, *parray;

    // calculating g(q)
    current_gq = calculateG_X(k, d, q_num, QUERY_FILE);
    pos_in_hash = customModulo(current_gq, hashtable_size);
    qarray = convertArray(query_images[q_num], d);
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
            parray = convertArray(all_images[HashTables[l][pos_in_hash][h].first], d);
            // calculate distance and g_p
            current_distance = manhattanDistance(qarray, parray, d);
            current_gp = HashTables[l][pos_in_hash][h].second;
            // delete the parray now
            delete[] parray;
            if (current_distance < min_distance && current_gq == current_gp) {
                n_neighbours.push_back(make_pair(HashTables[l][pos_in_hash][h].first, current_distance));
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
    delete[] qarray;
    
    auto finishLSH = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedLSH = finishLSH - startLSH;

    reverse(n_neighbours.begin(), n_neighbours.end());
    if (n_neighbours.size() > static_cast<unsigned int>(n)) n_neighbours.resize(n);
    // write in output file

    auto startTrue = chrono::high_resolution_clock::now();
    BNN = approximateN_NNs_Full_Search(d, n, q_num, number_of_images);
    auto finishTrue = chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTrue = finishTrue - startTrue;

    (*file) << "Query: " << q_num << endl;
    for (unsigned int i = 0; i < n_neighbours.size(); i++) {
        (*file) << "Nearest neighbour-" << i+1 << ": " << n_neighbours[i].first << endl;
        (*file) << "distanceLSH: " << n_neighbours[i].second << endl;
        (*file) << "distanceTrue: " << BNN[i].second << endl;
    }
    (*file) << "tLSH: " << elapsedLSH.count() << endl;
    (*file) << "tTrue: " << elapsedTrue.count() << endl;
    BNN.clear();
    n_neighbours.clear();
}

// searches using range search
void rangeSearch(ofstream* file, uint64_t d, int k, int L, uint32_t q_num, unsigned int radius, int number_of_images) {
    vector<pair <unsigned int, unsigned int> > n_neighbours;
    vector<pair <unsigned int, unsigned int> >::iterator it;
    unsigned int current_gp = 0, current_gq = 0;
    unsigned int current_distance = 0;
    int pos_in_hash = 0;
    int hashtable_size = number_of_images / HASHTABLE_NUMBER;
    // we start putting neighbours from farthest to closest
    int* qarray, *parray;

    // calculating g(q)
    current_gq = calculateG_X(k, d, q_num, QUERY_FILE);
    pos_in_hash = customModulo(current_gq, hashtable_size);
    qarray = convertArray(query_images[q_num], d);
    if (pos_in_hash > hashtable_size - 1) {
        // then something went wrong with g(p)
        cerr << "Calculating g(q) went wrong" << endl;
        exit(ERROR);
    }
    // for all hash_tables
    for (int l = 0; l < L; l++) {
        // loop over the bucket
        for (unsigned int h = 0; h < HashTables[l][pos_in_hash].size(); h++) {
            // calculate the Manhattan distance of q and every other image in the bucket
            // distance between HashTables[l][pos_in_hash][h], and query_image[q_num]
            parray = convertArray(all_images[HashTables[l][pos_in_hash][h].first], d);
            // calculate distance and g_p
            current_distance = manhattanDistance(qarray, parray, d);
            current_gp = HashTables[l][pos_in_hash][h].second;
            // delete the parray now
            delete[] parray;
            if (current_distance < radius && current_gq == current_gp) {
                // search if there is already inside
                // using lambda function
                unsigned int temp = HashTables[l][pos_in_hash][h].first;
                it = find_if(
                    n_neighbours.begin(),
                    n_neighbours.end(),
                    [&temp](const pair<unsigned int, unsigned int> &current_pair)
                    { return current_pair.first == temp; });
                // if item NOT exists, then push this item in the vector
                if(it == n_neighbours.end()) {
                    n_neighbours.push_back(make_pair(temp, current_distance));
                }
            }
            // take into account a maximum of (10 * L) points in each hashtable
            if (h > static_cast<unsigned int>(10*L)) {
                // break the loop to continue with next hashtable
                break;
            }
        }
    }
    delete[] qarray;
    
    sort(n_neighbours.begin(), n_neighbours.end(), [](const pair<unsigned int, unsigned int> &left, const pair<unsigned int, unsigned int> &right) {
        return left.second < right.second;
    });

    (*file) << "R-near neighbours:" << n_neighbours.size() << endl;
    for (unsigned int i=0; i<n_neighbours.size(); i++) {
        (*file) << n_neighbours[i].first << endl;
    }
    n_neighbours.clear();
}