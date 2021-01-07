#include <algorithm>
#include <stdint.h>

unsigned int manhattanDistance(int* x, int* y, uint64_t n) {
	unsigned int sum =0;
    for (uint64_t i = 0; i < n; i++) {
        sum += abs(x[i] - y[i]); 
    }
    return sum;
}