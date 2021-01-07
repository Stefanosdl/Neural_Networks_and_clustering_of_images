#include <stdint.h>
#include <vector>
#include <string>
// random value for now find the best
#define SMALL_E 5 
#define MAX_LOOPS 50

using namespace std;

extern vector<pair<int, unsigned int> > nearest_clusters;
// in the centroid vectors, store the indexes, 
// which correspond to each image
// To access the array with the dimensions, use cluster_images[i]

extern vector<int*> initial_centroids;
extern vector<int*> current_centroids;
extern vector<int*> previous_centroid;
// in cluster images, I save number_of_images images (is a d-dimensional array)

vector<pair<int*, vector<int> > > kmeansPP(int, uint32_t, uint64_t, int**);
vector<double> silhouette(vector<pair<int *, vector<int>>>, u_int64_t, int**);
unsigned int manhattanDistance(int*, int*, uint64_t);
