#include <algorithm>
#include <numeric> // std::iota
#include <vector>
#include "../headers/wassersten.h"
using namespace std;

vector<size_t> argsort(const vector<int> &v) {
    vector<size_t> idx;
    iota(idx.begin(), idx.end(), 0);
    // sort indexes based on values in v
    sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2){ return v[i1] < v[i2]; });
    return idx;
}

vector<int> diff(const vector<int> &v) {
    vector<int> delta;
    for (size_t i = 0; i < v.size() - 1; ++i) {
        delta.push_back(v[i + 1] - v[i]);
    }
    return delta;
}

vector<int> concatenateSort(const vector<int> &A, const vector<int> &B) {
    vector<int> C;
    C.reserve(A.size() + B.size());
    C.insert(C.end(), A.begin(), A.end());
    C.insert(C.end(), B.begin(), B.end());
    sort(C.begin(), C.end());
    return C;
}

vector<size_t> searchSorted(const vector<int> &v, const vector<size_t> &idx, const vector<int> &allValues) {
    vector<size_t> cdfIdx;
    // for every element in all indices, find id in A
    cdfIdx.reserve(allValues.size());
    for (auto elem = allValues.begin(); elem != allValues.end() - 1; ++elem) {
        auto up = upper_bound(idx.begin(), idx.end(), *elem,
                              [&v](int elem, size_t i) { return elem < v[i]; });
        if (up == idx.end()) {
            cdfIdx.push_back(idx.size());
        }
        else {
            cdfIdx.push_back(up - idx.begin());
        }
    }
    return cdfIdx;
}

vector<int> computeCDF(const vector<int> &weights, vector<size_t> &idx, const vector<size_t> &cdfIdx) {
    vector<int> cdf;
    vector<int> ordW;
    for (auto i : idx) {
        ordW.push_back(weights[i]);
    }

    cdf.reserve(1 + ordW.size());
    vector<int> sortedAccW;
    sortedAccW.push_back(0);
    partial_sum(ordW.begin(), ordW.end(), ordW.begin());
    sortedAccW.insert(sortedAccW.end(), ordW.begin(), ordW.end());
    int accum = sortedAccW.back();
    for (auto cIdx: cdfIdx) {
        cdf.push_back(sortedAccW[cIdx] / accum);
    }
    return cdf;
}

double computeDist(const vector<int> &cdfA, const vector<int> &cdfB, const vector<int> &deltas) {
    double dist = 0.0;
    for (size_t i = 0; i < deltas.size(); ++i) {
        dist += abs(cdfA[i] - cdfB[i]) * deltas[i];
    }
    return dist;
}

double wasserstein(vector<int> &A, vector<int> AWeights, vector<int> &B, vector<int> BWeights) {
    vector<int> allValues, deltas, cdfA, cdfB;
    vector<size_t> cdfIdxA, cdfIdxB;
    vector<size_t> idxA(A.size());
    vector<size_t> idxB(B.size());

    allValues = concatenateSort(A, B);
    deltas = diff(allValues);

    idxA = argsort(A);
    idxB = argsort(B);

    cdfIdxA = searchSorted(A, idxA, allValues);
    cdfIdxB = searchSorted(B, idxB, allValues);
    cdfA = computeCDF(AWeights, idxA, cdfIdxA);
    cdfB = computeCDF(BWeights, idxB, cdfIdxB);

    return computeDist(cdfA, cdfB, deltas);
}
// g++ main.cpp wasserstein.cpp -o test -I ../headers/wassersten.h