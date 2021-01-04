#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include "../headers/handle-input.hpp"
using namespace std;

unsigned int manhattanDistance(int* x, int* y, uint64_t n) {
	unsigned int sum =0;
    for (uint64_t i = 0; i < n; i++) {
        sum += abs(x[i] - y[i]); 
    }
    return sum;
}

// Brute Force
void Brute_Force(uint64_t d_original, uint32_t q_num, int number_of_images, unordered_map<int, vector<int> > &Brute) {
    vector<pair <int, unsigned int> > n_neighbours;
    vector<int> neighbours;
    unsigned int current_distance = 0;

    // loop over the images array
    for (int i=0; i<number_of_images; i++) {
        current_distance = manhattanDistance(query_images_original_space[q_num], all_images_original_space[i], d_original);
        n_neighbours.push_back(make_pair(i, current_distance));
    }
    sort(n_neighbours.begin(), n_neighbours.end(), [](const pair<unsigned int, unsigned int> &left, const pair<unsigned int, unsigned int> &right) {
        return left.second < right.second;
    });
    if (n_neighbours.size() > static_cast<unsigned int>(N)) n_neighbours.resize(N);

    for (unsigned int i=0; i<n_neighbours.size(); i++) {
        neighbours.push_back(n_neighbours[i].first);
    }
    Brute[q_num] = neighbours;
    // neighbours.clear();
}

void Evaluate_Results(int number_of_query_images, unordered_map<int, vector<int> > &Brute, ofstream* file) {
    // Number of images to be evaluated
    int sum = 0;
    double total_value = 0.0;
    for (int i=0; i<number_of_query_images; i++) {
        // Number of neighbours per image
        for (int j=0; j<N; j++) {
            if (all_images_labels[Brute[i][j]] == query_images_labels[i]) {
                sum++;
            }
        }
    }
    total_value = (double)sum/((double)number_of_query_images*(double)N);
    // write to file
    (*file) << "Average Correct Search Results MANHATTAN: " << total_value << endl;
}
