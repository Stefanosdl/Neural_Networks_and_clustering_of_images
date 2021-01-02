#include <iostream>
#include <algorithm>
#include <numeric> // std::iota
#include <vector>
#include "../headers/wasserstein.hpp"
using namespace std;

void argsort(const vector<int> &v, vector<int> &idx) {
    iota(idx.begin(), idx.end(), 0);
    // sort indexes based on values in v
    sort(idx.begin(), idx.end(), [&v](int i1, int i2){ return v[i1] < v[i2]; });
}

vector<int> diff(const vector<int> &v) {
    vector<int> delta;
    for (unsigned int i = 0; i < v.size() - 1; ++i) {
        delta.push_back(v[i + 1] - v[i]);
    }
    return delta;
}

void concatenateSort(const vector<int> &A, const vector<int> &B, vector<int> &C) {
    C.reserve(A.size() + B.size());
    C.insert(C.end(), A.begin(), A.end());
    C.insert(C.end(), B.begin(), B.end());
    sort(C.begin(), C.end());
}

void searchSorted(const vector<int> &v, const vector<int> &idx, const vector<int> &allValues, vector<int> &cdfIdx) {
    cdfIdx.reserve(allValues.size());
    for (auto elem = allValues.begin(); elem != allValues.end() - 1; ++elem) {
        auto up = upper_bound(idx.begin(), idx.end(), *elem,
                              [&v](int elem, int i) { return elem < v[i]; });
        if (up == idx.end()) {
            cdfIdx.push_back(idx.size());
        }
        else {
            cdfIdx.push_back(up - idx.begin());
        }
    }
}

void computeCDF(const vector<int> &weights, vector<int> &idx, const vector<int> &cdfIdx, vector<double> &cdf) {
    vector<int> ordW;
    for (unsigned int i = 0; i < idx.size(); i++) {
        ordW.push_back(weights[i]);
    }
    cdf.reserve(1 + ordW.size());
    vector<int> sortedAccW;
    sortedAccW.push_back(0);
    partial_sum(ordW.begin(), ordW.end(), ordW.begin());
    sortedAccW.insert(sortedAccW.end(), ordW.begin(), ordW.end());
    int accum = sortedAccW.back();

    for (unsigned int i = 0; i < cdfIdx.size(); i++) {
        cdf.push_back((double)sortedAccW[cdfIdx[i]] / (double)accum);
    }
}

double computeDist(const vector<double> &cdfA, const vector<double> &cdfB, const vector<int> &deltas) {
    double dist = 0.0;
    for (unsigned int i = 0; i < deltas.size(); ++i) {
        dist += abs(cdfA[i] - cdfB[i]) * deltas[i];
    }
    return dist;
}

double wasserstein(vector<int> &A, vector<int> AWeights, vector<int> &B, vector<int> BWeights) {
    vector<int> allValues, deltas;
    vector<double> cdfA, cdfB;
    vector<int> cdfIdxA, cdfIdxB;
    vector<int> idxA(A.size());
    vector<int> idxB(B.size());

    concatenateSort(A, B, allValues);
    deltas = diff(allValues);

    argsort(A, idxA);
    argsort(B, idxB);

    searchSorted(A, idxA, allValues, cdfIdxA);
    searchSorted(B, idxB, allValues, cdfIdxB);

    computeCDF(AWeights, idxA, cdfIdxA, cdfA);
    computeCDF(BWeights, idxB, cdfIdxB, cdfB);
    return computeDist(cdfA, cdfB, deltas);
}
