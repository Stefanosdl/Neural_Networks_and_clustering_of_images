#include <iostream>
#include <random>
#include "../headers/common.hpp"
#include "../headers/manhattan-hashing.hpp"
#include "../headers/handle-input.hpp"
#include "../headers/modulo.hpp"
#include "../headers/search.hpp"
#include "../headers/distances.hpp"
using namespace std;

int* convertArray(unsigned char* array, int size) {
    int *result_array = new int [size];
    for (int i = 0; i < size;i++) {
        result_array[i] = static_cast<int>(array[i]);
    }
    return result_array;
}

void calculateW_Component(uint64_t d, uint32_t number_of_images, uint32_t number_of_query_images) {
	int* qarray, *parray;
	auto sum = 0;
	for (uint32_t q=0; q<50; q++){
		for (uint32_t image=0; image<number_of_images; image++){
			qarray = convertArray(query_images[q], d);
			parray = convertArray(all_images[image], d);
			sum += manhattanDistance(qarray, parray, d);
			delete[] parray;
			delete[] qarray;
		}
	}
	w = sum / (50*number_of_images);
}

float calculateS_IComponent() {
	float s_i;
	random_device generator;
	uniform_real_distribution<float> distribution (0.0, (float) w);

	// Generate a new double number
	s_i = distribution(generator);
	return s_i;
}

int calculateA_IComponent(float p_i, uint64_t d) {
	// with this function we will calcuate the a_i components
	// for an image
	float sub, div;
	int a_i;
	float s_i;
	// calculate s_i
	s_i = calculateS_IComponent();
	// ADD w, to all p_i, this will move all of the p_i s away from the start of axis
	// Will remove the problem of having negative numbers in the a_i components!
	sub = p_i + w - s_i;
	// divide the subtraction with the w, which is globally accessible
	div = sub / w;
	// find the floor of the division
	a_i = static_cast<int>(floor(static_cast<double>(div)));
	return a_i;
}

unsigned long calculateH_XComponent(uint64_t d, unsigned char* x_i_array) {
	unsigned long int hx = 0;
	unsigned long long int mExp;
	int tempai, temphx;
	int a_i;
	// calculate each component of current h(x)
	for (uint64_t i = 0; i < d; i++) {
		// m^x%M
		a_i = calculateA_IComponent(static_cast<float>(x_i_array[i]), i);
		mExp = exponentiationModulo(m, d-(i+1), M);
		tempai = customModulo(a_i, M);
		temphx = customModulo(mExp*tempai, M);
		hx += customModulo(temphx, M);
	}
	
	hx = customModulo(hx, M);
	return hx;
}

// Calculate h(x) using the formula from the theory
int createH_X (uint64_t d, int image, int file_type) {
	// then calculate hx
	int hx = file_type == INPUT_FILE ? \
		calculateH_XComponent(d, all_images[image]) : \
		calculateH_XComponent(d, query_images[image]);
    return hx;
}

// Calculate g(x) using bitwise operations
unsigned int calculateG_X (int k, int d, uint32_t image, int file_type) {
	int h_x;
	unsigned int g_x = 0;
    // That is the max number of bits of the h(x)
    int shift = 32/k;
	for (int i=0; i<k; i++) {
        // create h_x
    	h_x = createH_X(d, image, file_type);
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