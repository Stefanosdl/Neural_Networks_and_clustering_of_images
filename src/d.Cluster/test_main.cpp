#include <iostream>
#include <fstream>
#include "./headers/common.hpp"
#include "./headers/handle-input.hpp"

int main(void) {
    readClusterFile("clusterFile.txt", 10);
    return 0;
}