#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <cmath>
#include "headers/kmeansPP/kmeansPP.hpp"
#include "headers/common.hpp"

using namespace std;

int main(int argc, char **argv) {
    string output_file;
    string input_file;
    string config_file;
    int K_medians = -1;
    int L = 3;
    int k_LSH = 4;
    uint32_t number_of_images = 0;
    uint64_t d = 0;
    vector<pair<int*, vector<int> > > clusters;
    vector<double> s_i;
    ofstream o_file;

    handleInput(argc, argv, &number_of_images, &d_original, &K_medians, &k, &l, &output_file);

    w=400;
    M = pow(2, 32/(k_LSH));

    m = M/2 - 1;
    auto start = chrono::system_clock::now();
    clusters = kmeansPP(L, k_LSH, K_medians, number_of_images, d, method, points_M, probes);
    s_i = silhouette(clusters, d);

    auto end = chrono::system_clock::now();
    auto elapsed = chrono::duration<double>(end - start);

    // open output file
    o_file.open(output_file);
    if (!o_file.is_open()) {
        cerr << "Output file can't be opened" << endl;
        exit(ERROR);
    }

    // Write to file 
    o_file << "Algorithm: " << method << endl;
    for (unsigned int i=0; i < clusters.size(); i++) {
        if (clusters[i].second.size() == 0 || clusters[i].first == NULL) continue;
        if (complete_flag) {
            o_file << "CLUSTER-" << i+1 << " { centroid: [";
            for (uint64_t j=0; j < d - 1; j++) {
                o_file << clusters[i].first[j] << ", ";
            }
            o_file << clusters[i].first[d - 1] << "], [";
            for (uint64_t j=0; j < clusters[i].second.size() - 1; j++) {
                o_file << clusters[i].second[j] << ", ";
            }
            o_file <<clusters[i].second[clusters[i].second.size() - 1] << "]" << endl;
        }
        else {
            o_file << "CLUSTER-" << i+1 << " {size: " << clusters[i].second.size() << ", centroid: [";
            for (uint64_t j=0; j < d - 1; j++) {
                o_file << clusters[i].first[j] << ", ";
            }
            o_file << clusters[i].first[d - 1] << "] }" << endl;
        }
    }
    o_file << "clustering_time: " << elapsed.count() << " seconds" <<endl;

    o_file << "Silhouette: [";
    for (unsigned int i=0; i < s_i.size() - 1; i++)
        o_file << s_i[i] << ", ";
    o_file << s_i[s_i.size() - 1] << "]";

    o_file.close();
    s_i.clear();
    clusters.clear();
    return SUCCESS;
}