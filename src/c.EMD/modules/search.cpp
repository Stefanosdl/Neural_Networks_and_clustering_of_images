#include <iostream>
#include <vector>
#include <algorithm>
#include "../headers/wasserstein.hpp"
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
vector<pair <int, unsigned int> > Brute_Force(uint64_t d_original, int n, uint32_t q_num, int number_of_images) {
    vector<pair <int, unsigned int> > n_neighbours;
    unsigned int current_distance = 0;

    // loop over the images array
    for (int i=0; i<number_of_images; i++) {
        current_distance = manhattanDistance(query_images_original_space[q_num], all_images_original_space[i], d_original);
        n_neighbours.push_back(make_pair(i, current_distance));
    }
    sort(n_neighbours.begin(), n_neighbours.end(), [](const pair<unsigned int, unsigned int> &left, const pair<unsigned int, unsigned int> &right) {
        return left.second < right.second;
    });
    if (n_neighbours.size() > static_cast<unsigned int>(n)) n_neighbours.resize(n);

    return n_neighbours;
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