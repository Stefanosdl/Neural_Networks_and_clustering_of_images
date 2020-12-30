#include <iostream>
#include <numeric> // std::iota
#include "../headers/wassersten.h"
using namespace std;
double EMDdistance(int* query_image, size_t size_query_image, int* test_image, size_t size_test_image);

int main(int argc, char **argv) {
    int query_image[] = {0,1,3};
    int test_image[] = {5,6,8};
    // ............... Call EMD
    size_t size_q = sizeof query_image;
    size_t size_t = sizeof test_image;
    cout << EMDdistance(query_image, size_q, test_image, size_t) << endl;
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