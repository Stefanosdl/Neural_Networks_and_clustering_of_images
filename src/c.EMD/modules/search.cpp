#include <iostream>
#include <vector>
#include <set>
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
void Brute_Force(uint64_t d_original, uint32_t q_num, int number_of_images, set<vector<int> > &Brute) {
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

    Brute.insert(neighbours);
    neighbours.clear();
}
