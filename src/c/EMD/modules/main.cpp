#include <iostream>
#include <numeric> // std::iota

#include "../headers/wassersten.h"
using namespace std;

int main(int argc, char **argv) {
    vector<int> aw;
    vector<int> av;

    vector<int> bw;
    vector<int> bv;

    aw.push_back(1);
    aw.push_back(1);
    aw.push_back(1);
    av.push_back(0);
    av.push_back(1);
    av.push_back(3);
    iota(av.begin(), av.end(), 0);

    bw.push_back(1);
    bw.push_back(1);
    bw.push_back(1);
    bv.push_back(5);
    bv.push_back(6);
    bv.push_back(8);
    iota(bv.begin(), bv.end(), 0);
    double dist;

    dist = wasserstein(av,aw,bv,bw);
    cout << "The earth movers distance is: " << dist << endl;
}