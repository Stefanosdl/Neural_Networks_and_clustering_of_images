#include <iostream>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <bitset>
#include "../../headers/kmeansPP/kmeansPP.hpp"
#include "../../headers/distances.hpp"
#include "../../headers/modulo.hpp"
#include "../../headers/manhattan-hashing.hpp"
#include "../../headers/common.hpp"

using namespace std;

// Store the closest centroid index and distance for every image
vector<pair<int, unsigned int> > nearest_clusters;
// every index in vector stores its coordinates in an int* array
// in order to use it to manhattan distance
vector<int*> initial_centroids;
vector<int*> current_centroids;
vector<int*> previous_centroids;
// Hashtables for LSH
vector<vector<vector<pair<int, unsigned int> > > > HashTablesKmeans;
// Hypercube
multimap <string, int> HypercubeKmeans;
vector<unordered_map<int, int> > projections;


int getRandomNumber(int number) {
	random_device generator;
	uniform_int_distribution<int> distribution (0, number - 1);
	return distribution(generator);
}

double getRealRandomNumber(double number) {
    random_device generator;
    uniform_real_distribution<double> distribution (0.0, number);
    return distribution(generator);
}

float getFloatRandomNumber(float number) {
    random_device generator;
    uniform_real_distribution<float> distribution (0.0, number);
    return distribution(generator);
}

vector<int> getImagesInCluster(int centroid_idx) {
    vector<int> images_in_cluster;
    for (unsigned int i = 0; i < nearest_clusters.size(); i++) {
        if(nearest_clusters[i].first == centroid_idx) {
            images_in_cluster.push_back(i);
        }
    }
    return images_in_cluster;
}

bool isSameCoords(int* arr1, int* arr2, uint64_t d) {
    for (uint64_t i = 0; i < d; i++) {
        if(arr1[i] != arr2[i]) {
            return false;
        }
    }
    return true;
}

int nextInitialCentroidIndex(uint32_t number_of_images, uint64_t d) {
    unsigned int min_dist, max_dist, current_distance = 0;
    vector<float> partial_sums;
    float d_i = 0.0;
    float x = 0.0;
    partial_sums.push_back(0.0);
    for (uint32_t i = 0; i < number_of_images; i++) {
        min_dist = inf;
        max_dist = 0;
        for (unsigned int centroid = 0; centroid < initial_centroids.size(); centroid++) {
            if (isSameCoords(cluster_images[i], initial_centroids[centroid], d)) continue;
            current_distance = manhattanDistance(initial_centroids[centroid], cluster_images[i], d);
            if (current_distance < min_dist) {
                min_dist = current_distance;
            }
            if (current_distance > max_dist) {
                max_dist = current_distance;
            }
        }
        if (min_dist == inf) continue;
        d_i = (float)((float)min_dist/(float)max_dist);
        d_i = pow(d_i, 2);
        partial_sums.push_back(d_i + partial_sums.back());
    }
    x = getFloatRandomNumber(partial_sums.back());
    for (unsigned int r = 1; r < partial_sums.size() - 1; r++) {
        if (x > partial_sums[r] && x <= partial_sums[r+1]) {
            return (int)(r+1);
        }
    }
    return partial_sums.back();
}

void getClosestCentroid(uint32_t index, vector<int*> centroids, uint64_t d) {
    int closest_cluster = -1;
    unsigned int min_distance = inf;
    for (unsigned int c = 0; c < centroids.size(); c++) {
        unsigned int distance = manhattanDistance(centroids[c], cluster_images[index], d);
        if (distance < min_distance) {
            min_distance = distance;
            closest_cluster = c;
        }
    }
    if (closest_cluster == -1) {
        cerr << "Closest Cluster didn't update correctly" << endl;
        exit(ERROR);
    }
    nearest_clusters[index] = make_pair(closest_cluster, min_distance);
}

// calculate distance for every image for each centroid and
// select the smallest distance and save it in this cluster's centroid
// This function should have three different bodies
// the current body represent the Lloyd's, the other two will be the reverse LSH and reverse Cube
void updateNearClusters(vector<int*> centroids, uint32_t number_of_images, uint64_t d) {
    for (uint32_t i = 0; i < number_of_images; i++) {
        getClosestCentroid(i, centroids, d);        
    }
}

// k-medians, calculate the median in order to get next 
// cluster's centroid coords
void updateCentroids(vector<int*> &centroids, uint32_t number_of_images, uint64_t d) {
    vector<int> temp_array;
    vector<int *> new_centroids;
    vector<int> images_in_cluster;

    new_centroids.resize(centroids.size());

    for (unsigned int i=0; i< new_centroids.size(); i++) {
        new_centroids[i] = new int [d];
    }

    for (unsigned int c = 0; c < centroids.size(); c++) {
        // for loop for every image in the cluster c
        // call function that returns a vector with all the images in a given cluster, by centroid index
        images_in_cluster = getImagesInCluster(c);
        // make a for loop for every dimension of the d
        for (uint64_t j = 0; j < d; j++) {
            temp_array.resize(images_in_cluster.size());
            for (unsigned int img = 0; img < images_in_cluster.size(); img++) {
                int p = cluster_images[images_in_cluster[img]][j];
                temp_array[img] = p;
            }
            // find median
            sort(temp_array.begin(), temp_array.end());
            new_centroids[c][j] = temp_array[temp_array.size() / 2];
            temp_array.clear();
        }
    }
    // we have new_centroids array ready, with updated values
    centroids = new_centroids;
    images_in_cluster.clear();
}

bool equalCentroids(uint64_t d, int loops) {
    unsigned int total_distance = 0;
    if (loops > MAX_LOOPS) return true;
    for (unsigned int i = 0; i < current_centroids.size(); i++) {
        total_distance += manhattanDistance(current_centroids[i], previous_centroids[i], d);
    }
    return (total_distance < SMALL_E);
}

vector<pair<int*, vector<int> > > kmeansPP(int L, int k_LSH, int K, uint32_t number_of_images, uint64_t d, string method, int points_M, int probes) {
    // this is a vector of all centroids
    // for each centroid store a vector of images assigned in this cluster
    vector<vector<int> > temp(K);
    vector<pair<int*, vector<int> > > clusters;
    int loops = 0;
    // Get a random centroid
    int* first_centroid;
    
    first_centroid = cluster_images[getRandomNumber(number_of_images)];
    initial_centroids.push_back(first_centroid);
    // it will have one position for each image
    nearest_clusters.resize(number_of_images);
    if (method == "LSH") {
        initializeHashtablesKmeans(L, number_of_images);
        putImagesInHashtables(number_of_images, L, k_LSH, d);
    }
    else if (method == "HYPERCUBE") {
        putImagesInHypercubeKmeans(number_of_images, d);
    }

    // Initialize the K centroids
    for (int i = 1; i < K; i++) {
        updateNearClusters(initial_centroids, number_of_images, d);
        initial_centroids.push_back(cluster_images[nextInitialCentroidIndex(number_of_images, d)]);
    }

    current_centroids = initial_centroids;

    do {
        previous_centroids = current_centroids;
        if (method != "CLASSIC") {
            reverseAssignment(method, number_of_images, L, k_LSH, d, points_M, probes) ;
        }
        else {
            updateNearClusters(current_centroids, number_of_images, d);
        }
        updateCentroids(current_centroids, number_of_images, d);
        loops++;
    } while (!equalCentroids(d, loops));

    // finally make the data for silhouette
    for (uint32_t i = 0; i < number_of_images;i++)
        temp[nearest_clusters[i].first].push_back(i);
    for (unsigned int i = 0; i < current_centroids.size(); i++)
        clusters.push_back(make_pair(current_centroids[i], temp[i]));
    return clusters;
}

// -------------------------------------------------------BEGIN REVERSE ASSIGNMENT

unsigned char* reConvertArray(int* array, uint64_t d) {
    unsigned char* result = new unsigned char[d];
    for (uint64_t i = 0; i < d; i++) {
          result[i] = static_cast<unsigned char>(array[i]);
    }
    return result;
}

// Calculate g(x) using bitwise operations
unsigned int calculateG_XforKmeans(int k, uint64_t d, uint32_t image, int type) {
	int h_x = 0;
	unsigned int g_x = 0;
    // That is the max number of bits of the h(x)
    int shift = 32/k;
	for (int i=0; i<k; i++) {
        // create h_x
        switch(type) {
            case CLUSTER_IMAGES :
                h_x = calculateH_XComponent(d, reConvertArray(cluster_images[image], d));
                break;
            case CENTROIDS :
                h_x = calculateH_XComponent(d, reConvertArray(current_centroids[image], d));
                break;
        }
        if (i == 0) {
            g_x = h_x;
        }
        else {
    		g_x <<= shift;
        	g_x |= h_x;
    	}
    }
    return g_x;
}

void initializeHashtablesKmeans(int L, uint32_t n) {
    int hashtable_size = n/HASHTABLE_NUMBER_K_MEANS;
    HashTablesKmeans.resize(L, vector<vector<pair<int, unsigned int> > >(hashtable_size, vector<pair<int, unsigned int> >(0)));
}

void insertToHashtableKmeans(int L, uint32_t image, unsigned int g_x, uint32_t n) {
	int hashtable_size = n/HASHTABLE_NUMBER_K_MEANS;
	unsigned long pos = customModulo(g_x, hashtable_size);
	HashTablesKmeans[L][pos].push_back(make_pair(static_cast<int>(image), g_x));
}

void putImagesInHashtables(uint32_t number_of_images, int L, int k, uint64_t d) {
    unsigned int g_x = 0;
    for (uint32_t img = 0; img < number_of_images; img++){
        for (int l = 0; l < L; l++) {
            g_x = calculateG_XforKmeans(k, d, img, CLUSTER_IMAGES);
            // pass to hashtable
            insertToHashtableKmeans(l, img, g_x, number_of_images);
        }

    }
}

void rangeSearchLSH(int L, uint64_t d, int centroidIndex, int radius, vector<pair<int, unsigned int> > centroids_gx, vector<bool> &assigned_images) {
    int pos_in_hash = centroids_gx[centroidIndex].first;
    unsigned int current_distance = 0;
    int* centroids_coordinates = new int[d];
    centroids_coordinates = current_centroids[centroidIndex];

    // for all hash_tables
    for (int l = 0; l < L; l++) {
        // loop over the bucket
        for (unsigned int h = 0; h < HashTablesKmeans[l][pos_in_hash].size(); h++) {
            // calculate distance and g_p
            current_distance = manhattanDistance(centroids_coordinates, cluster_images[HashTablesKmeans[l][pos_in_hash][h].first], d);
            if (current_distance < (unsigned int)radius) {
                unsigned int temp = HashTablesKmeans[l][pos_in_hash][h].first;
                // first check the image is already assigned or the distance is closer to current centroid
                if (!assigned_images[temp] || current_distance < nearest_clusters[temp].second) {
                    // assign image to the new cluster 
                    nearest_clusters[temp] = make_pair(centroidIndex, current_distance);
                    // flag image as assigned
                    assigned_images[temp] = true;
                }
            }
        }
    }
    delete[] centroids_coordinates;
}

// the radius will be: min dist between two closest centers
int calculateMinDistOfTwoCentroids(uint64_t d) {
    int result = 0;
    vector<unsigned int> distances;
    for (unsigned int i = 0; i < current_centroids.size() - 1; i++) {
        for (unsigned int j = i+1; j < current_centroids.size(); j++) {
            // cout<<current_centroids[i]<<"  "<<current_centroids[j]<<endl;
            distances.push_back(manhattanDistance(current_centroids[i], current_centroids[j], d));
        }
    }
    sort(distances.begin(), distances.end());
    result = (int)distances[0];
    distances.clear();
    return result;
}

// check the rate with which new points are push in the assigned_images
int newPointsAssigned;
bool fewNewPoints(vector<bool> assigned_images, int minRate) {
    // count all values that are true
    int allAssigned = count(assigned_images.begin(), assigned_images.end(), true);
    newPointsAssigned = allAssigned - newPointsAssigned;
    return (newPointsAssigned <= minRate);
}

/* `type` will be LSH or HyperCube
    and depending on that 
    We call:
    * rangeSearchLSH
    * rangeSearchHyperCube
    in the loop
*/

void reverseAssignment(string method, uint32_t number_of_images, int L, int k, uint64_t d, int points_M, int probes) {
    // the assigned_images vector has number_of_images positions and in each position is the information 
    // of having been assigned in a cluster or not
    vector<bool> assigned_images(number_of_images, false);
    // initialize the global variable with 0
    newPointsAssigned = 0;
    // if the new points assigned are less than 2 * the number of the centroids
    // then we can skip the do while and range search and perform Lloyd's
    // we can change this minrate to current_centroids.size()
    int minRate = current_centroids.size()*2;
    // and multipy it by 2 until most balls get no new points
    int radius = calculateMinDistOfTwoCentroids(d)/2;
    // for example for radius = 3, max_radius = 96, so 4 loops (16 = 2^4)
    // the loops we want is the power of 2 we are going to multiply the initial radius with
    int MAX_RADIUS = 16*radius; 
    // when we finish with the first radius value rerun with bigger values and assign unassigned spots only!
    // the assignment will happen like above in the `nearest_clusters` vector.

    // ---------------------------LSH
    vector<pair<int, unsigned int> > centroids_gx;
    unsigned int current_gx = 0;
    int pos_in_hash = 0;
    int hashtable_size = number_of_images / HASHTABLE_NUMBER_K_MEANS;
    // ---------------------------Hypercube
    vector<string> centroids_gx_Hypercube;
    string current_gx_Hypercube;
    int d_space = (int)(log2(d) - 1);

    // ---------------------------Init structures
    if(method == "LSH") {
        for (unsigned int c = 0; c < current_centroids.size(); c++) {
            current_gx = calculateG_XforKmeans(k, d, c, CENTROIDS);
            pos_in_hash = customModulo(current_gx, hashtable_size);
            if (pos_in_hash > hashtable_size - 1) {
                // then something went wrong with g(p)
                cerr << "Calculating g(centroid) went wrong" << endl;
                exit(ERROR);
            }
            centroids_gx.push_back(make_pair(pos_in_hash, current_gx));
        }
    }
    else if(method == "HYPERCUBE") {
        for (unsigned int c = 0; c < current_centroids.size(); c++) {
            current_gx_Hypercube = calculateCubeG_XKmeans(d, c, d_space, CENTROIDS);
            centroids_gx_Hypercube.push_back(current_gx_Hypercube);
        }
    }

    // Reverse assignment with LSH or Hypercube
    do {
        for (unsigned int c = 0; c < current_centroids.size(); c++) {
            if(method == "LSH") {
                rangeSearchLSH(L, d, c, radius, centroids_gx, assigned_images);
            }
            else if(method == "HYPERCUBE") {
                rangeSearchHyperCube(centroids_gx_Hypercube[c], c, d, points_M, probes, radius, assigned_images);
            }
        }
        // check if new clusters don't get a lot of new points and break here
        if(fewNewPoints(assigned_images, minRate)) break;
        radius *= 2;
    } while (radius < MAX_RADIUS);

    // for the ones left unassigned, calculate distances of all centroids and get the closest one
    for (unsigned int i=0; i < number_of_images; i++) {
        if (!assigned_images[i]) {
            getClosestCentroid(i, current_centroids, d);
        }
    }
    centroids_gx.clear();
}


// ---------------------------------------------------------Reverse Hypercube

void insertToHypercubeKmeans(string g_x, int image) {
	HypercubeKmeans.insert(make_pair(g_x, image));
}

string decimalToBinary(int n, int d_space) { 
    //finding the binary form of the number and  
    //converting it to string.  
    string s = bitset<64> (n).to_string();
    // keep d' bits
    s.erase(s.begin(), s.end() - d_space);
    return s;
}

int distribute_Bits() {
	random_device generator;
	uniform_int_distribution<int> distribution (0, 2);
	// Generate a new int number
	return distribution(generator);
}

string calculateCubeG_XKmeans(int d, int image, int d_space, int type) {
	int h_x;
	int bit;
	int shift = 1;
	int cubeG_X = 0;
	for (int i=0; i<d_space; i++) {
        switch(type) {
            case CLUSTER_IMAGES :
                h_x = calculateH_XComponent(d, reConvertArray(cluster_images[image], d));
                break;
            case CENTROIDS :
                h_x = calculateH_XComponent(d, reConvertArray(current_centroids[image], d));
                break;
        }
		// check if h_x exists in current f
		if (projections[i].find(h_x) == projections[i].end()) {
			bit = distribute_Bits();
			// it does not exists, so add it
			projections[i][h_x] = bit;
		}
		// now that we have f_i(h_i(p)) as a bit
		// append it in bitstring
		if (i == 0) {
			// if bit is set
			// then just shift
			cubeG_X = projections[i][h_x];
		}
		else{
			cubeG_X <<= shift;
			cubeG_X |= projections[i][h_x];
		}
	}
	return decimalToBinary(cubeG_X, d_space);
}

void putImagesInHypercubeKmeans(uint32_t number_of_images, uint64_t d) {
    string cube_g_x;
    int d_space = (int)(log2(d) - 1);
    projections.resize(d_space);
    for (uint32_t image = 0; image < number_of_images; image++) {
        cube_g_x = calculateCubeG_XKmeans(d, image, d_space, CLUSTER_IMAGES);
        // insert to hypercube
        insertToHypercubeKmeans(cube_g_x, image);
    }
}

// -------------------------------------

int hammingDistance(string queryHash, string bucketHash) {
	int i = 0, distance = 0;
	while(queryHash[i] != '\0') {
		if(queryHash[i] != bucketHash[i]) {
			distance++;
		}
		i++;
	}
	return distance;
}

// function to all values of duplicate keys in a multimap
vector<int> findImagesInBucket(string hash) {
	vector<int> result;
	auto search = HypercubeKmeans.equal_range(hash);
	for (auto i = search.first ; i != search.second ; ++i) {
		result.push_back(i->second);
	}
	return result;
}

// find strings that have hamming distance dist
vector<string> findHashWithSpecificHammingDist(string queryHash, int dist, int maximumProbes) {
	vector<string> res;
	multimap <string, int> :: iterator itr;
	int localProbes = maximumProbes;
	for (itr = HypercubeKmeans.begin(); itr != HypercubeKmeans.end(); ++itr) {
		if(hammingDistance(queryHash, itr->first) == dist) {
			// if it doesn't exist in vector, push it back
			if(find(res.begin(), res.end(), itr->first) == res.end()) {
				res.push_back(itr->first);
				localProbes--;
			}
			if(localProbes <= 0) break;
		}
	}
	return res;
}

vector<int> findAllNeighboursToBeChecked(string queryHash, int maximumN, int probes) {
	int currentHamming = 0;
	int numberOfProbesToCheck = probes;
	vector<string> currentNeighbourBuckets;
	vector<int> currentPossibleNeighbours;
	vector<int> allPossibleNeighbours;
	int flag = 1;
	do {
		currentNeighbourBuckets = findHashWithSpecificHammingDist(queryHash, currentHamming, numberOfProbesToCheck);
		for (unsigned int buck = 0; buck < currentNeighbourBuckets.size(); buck++) {
			currentPossibleNeighbours = findImagesInBucket(currentNeighbourBuckets[buck]);
			// checked some more
			numberOfProbesToCheck--;
			// now push back elements to allPossibleNeighbours
			for (unsigned int el = 0; el < currentPossibleNeighbours.size(); el++) {
				allPossibleNeighbours.push_back(currentPossibleNeighbours[el]);
				if (allPossibleNeighbours.size() >= (unsigned int)maximumN)
					flag = 0;
			}
			if (numberOfProbesToCheck <= 0)
				flag = 0;
			currentPossibleNeighbours.clear();
		}
		currentNeighbourBuckets.clear();
		currentHamming++;
	} while (flag);

	return allPossibleNeighbours;
}

// --------------------------------------

void rangeSearchHyperCube(string centroidHash, int centroidIndex, uint64_t d, int points_M, int probes, int radius, vector<bool> &assigned_images) {
    unsigned int current_distance = 0;
	vector<int> allPossibleNeighbours;
	allPossibleNeighbours = findAllNeighboursToBeChecked(centroidHash, points_M, probes);

	for (unsigned int i=0; i < allPossibleNeighbours.size(); i++) {
        current_distance = manhattanDistance(current_centroids[centroidIndex], cluster_images[allPossibleNeighbours[i]], d);
        if (current_distance < (unsigned int)radius) {
            int temp = allPossibleNeighbours[i];
            // first check the image is already assigned or the distance is closer to current centroid
            if (!assigned_images[temp] || current_distance < nearest_clusters[temp].second) {
                // assign image to the new cluster 
                nearest_clusters[temp] = make_pair(centroidIndex, current_distance);
                // flag image as assigned
                assigned_images[temp] = true;
            }
        }
	}
	allPossibleNeighbours.clear();
}

// -------------------------------------------------------END REVERSE ASSIGNMENT