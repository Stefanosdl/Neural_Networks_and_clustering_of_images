#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <cmath>
#include "headers/kmeansPP.hpp"
#include "headers/common.hpp"
#include "headers/handle-input.hpp"

using namespace std;

int main(int argc, char **argv) {
    string output_file;
    string input_file;
    string config_file;
    string clusters_file;
    int K_medians;
    uint32_t number_of_images = 0;
    uint64_t d = 0;
    uint64_t d_original = 0;
    vector<pair<int*, vector<int> > > cluster_from_file, clusters_original_space, clusters_new, clusters_new_space;
    vector<double> s_i_from_file, s_i_original, s_i_new;
    ofstream o_file;
    int* coords;

    handleInput(argc, argv, &number_of_images, &d_original, &d, &K_medians, &clusters_file, &output_file);
    // read clusters from file and prepare the struct for the silhouette
    // S3 clustering
    cluster_from_file = readClusterFile(clusters_file, K_medians, d_original);

    s_i_from_file = silhouette(cluster_from_file, d_original, all_images_original_space);
    cout<<"END SIL FILE"<<endl;
    // Original Space
    auto start = chrono::system_clock::now();
    clusters_original_space = kmeansPP(K_medians, number_of_images, d_original, all_images_original_space);
    s_i_original = silhouette(clusters_original_space, d_original, all_images_original_space);
    cout<<"END SIL ORIGINAL"<<endl;
    auto end = chrono::system_clock::now();
    auto elapsedOriginalSpace = chrono::duration<double>(end - start);
    // New Space  
    auto start = chrono::system_clock::now();
    clusters_new = kmeansPP(K_medians, number_of_images, d, all_images_new_space);
    // find centroids on original space
    for (int i = 0; i < K_medians; i++) {
        // Calculate the centroids
        coords = Calculate_Centroid(d_original, clusters_new[i].second);
        clusters_new_space.push_back(make_pair(coords, clusters_new[i].second));
    }
    s_i_new = silhouette(clusters_new_space, d_original, all_images_original_space);
    cout<<"END SIL NEW"<<endl;
    auto end = chrono::system_clock::now();
    auto elapsedNewSpace = chrono::duration<double>(end - start);

    // open output file
    o_file.open(output_file);
    if (!o_file.is_open()) {
        cerr << "Output file can't be opened" << endl;
        exit(ERROR);
    }

    // Write to file
    o_file << "NEW SPACE" <<endl;
    for (unsigned int i=0; i < clusters_new_space.size(); i++) {
        if (clusters_new_space[i].second.size() == 0 || clusters_new_space[i].first == NULL) {
            o_file << "CLUSTER-" << i+1 << " {size: " << clusters_new_space[i].second.size() << ", centroid: [] }";
            continue;
        }
        o_file << "CLUSTER-" << i+1 << " {size: " << clusters_new_space[i].second.size() << ", centroid: [";

        for (uint64_t j=0; j < d_original - 1; j++) {
            o_file << clusters_new_space[i].first[j] << ", ";
        }
        o_file << clusters_new_space[i].first[d_original - 1] << "] }" << endl;
    }
    o_file << "clustering_time: " << elapsedNewSpace.count() << " seconds" <<endl;

    o_file << "Silhouette: [";
    for (unsigned int i=0; i < s_i_new.size() - 1; i++)
        o_file << s_i_new[i] << ", ";
    o_file << s_i_new[s_i_new.size() - 1] << "]";

    o_file << "ORIGINAL SPACE" <<endl;
    for (unsigned int i=0; i < clusters_original_space.size(); i++) {
        if (clusters_original_space[i].second.size() == 0 || clusters_original_space[i].first == NULL) {
            o_file << "CLUSTER-" << i+1 << " {size: " << clusters_original_space[i].second.size() << ", centroid: [] }";
            continue;
        }
        o_file << "CLUSTER-" << i+1 << " {size: " << clusters_original_space[i].second.size() << ", centroid: [";

        for (uint64_t j=0; j < d_original - 1; j++) {
            o_file << clusters_original_space[i].first[j] << ", ";
        }
        o_file << clusters_original_space[i].first[d_original - 1] << "] }" << endl;
    }
    o_file << "clustering_time: " << elapsedOriginalSpace.count() << " seconds" <<endl;

    o_file << "Silhouette: [";
    for (unsigned int i=0; i < s_i_original.size() - 1; i++)
        o_file << s_i_original[i] << ", ";
    o_file << s_i_original[s_i_original.size() - 1] << "]";

    o_file << "CLASSES AS CLUSTERS" <<endl;
    o_file << "Silhouette: [";
    for (unsigned int i=0; i < s_i_from_file.size() - 1; i++)
        o_file << s_i_from_file[i] << ", ";
    o_file << s_i_from_file[s_i_from_file.size() - 1] << "]";

    o_file.close();
    s_i_from_file.clear();
    s_i_original.clear();
    s_i_new.clear();
    cluster_from_file.clear();
    clusters_original_space.clear();
    clusters_new_space.clear();

    return SUCCESS;
}