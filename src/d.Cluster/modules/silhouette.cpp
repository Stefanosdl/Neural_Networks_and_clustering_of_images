#include "../headers/kmeansPP.hpp"
#include "../headers/common.hpp"

unsigned int averageDistanceOfImageInCluster(int image, vector<pair<int*, vector<int> > > clusters, int c, uint64_t d, int** cluster_images) {
    unsigned int total_dist = 0;
    // for all images in this cluster
    // except yourself
    for (unsigned int i = 0; i < clusters[c].second.size(); i++){
        if (clusters[c].second[i] == image) {
            // it's myself so continue
            continue;
        }
        // cluster_images[image]: is the current image who we check its distances
        // clusters[c].second[i]: is the index of the next image to be checked
        // cluster_images[clusters[c].second[i]]: are the coords of the above mentined image
        total_dist += manhattanDistance(cluster_images[image], cluster_images[clusters[c].second[i]], d);
    }
    // There is only image in cluster
    if (clusters[c].second.size()== 1) {
        return 0;
    }
    // the loops above where (clusters[c].second.size() - 1), because it didn't run for it self
    return total_dist / (clusters[c].second.size() - 1);
}

// take a centroid and measure distance with all other centroids and return the coords of the closest one
int findSecondClosestCentroid (int* centroidCoords, vector<pair<int*, vector<int> > > clusters, uint64_t d) {
    unsigned int min_distance = inf;
    int res_centroid = -1;
    unsigned int dist = 0;
    for (unsigned int c = 0; c < clusters.size(); c++) {
        dist = manhattanDistance(centroidCoords, clusters[c].first, d);
        // if dist is 0 then we find the same centroid
        if (dist == 0) continue;
        if(dist < min_distance) {
            min_distance = dist;
            res_centroid = c;
        }
    }
    return res_centroid;
}

double calculateS_I (unsigned int a_i, unsigned int b_i) {
    return ((double)b_i - (double)a_i)/((double)a_i > (double)b_i ? (double)a_i : (double)b_i);
}

vector<double> silhouette(vector<pair<int*, vector<int> > > clusters, uint64_t d, int** cluster_images) {
    unsigned int a_i = 0, b_i = 0;
    double tmp_s_i;
    int counter = 0;
    // store the average s_i of each cluster c in one spot in this vector
    double final_s_i = 0;
    vector<double> s_i;

    for (unsigned int c = 0; c < clusters.size(); c++) {
        // for each image in this centroid "c" calculate the s(i)
        tmp_s_i = 0;
        for (unsigned int img = 0; img < clusters[c].second.size(); img++) {
            // calculate a_i component for this specific image
            a_i = averageDistanceOfImageInCluster(clusters[c].second[img], clusters, c, d, cluster_images);
            // calculate b_i component for this specific image
            b_i = averageDistanceOfImageInCluster(clusters[c].second[img], clusters, findSecondClosestCentroid(clusters[c].first, clusters, d), d, cluster_images);
            // add to the counter for the s_i calculation
            tmp_s_i += calculateS_I(a_i, b_i);
        }
        // calculate the average s(i) of all images in this cluster (centroid)
        // now in tmp_s_i I have the sum of all s_i for this cluster
        if (clusters[c].second.size() != 0) {
            counter++;
            tmp_s_i /= (double)clusters[c].second.size();
            s_i.push_back(tmp_s_i);
            final_s_i += tmp_s_i;
        }
    }

    final_s_i /= (double)counter;
    s_i.push_back(final_s_i);

    return s_i;
}