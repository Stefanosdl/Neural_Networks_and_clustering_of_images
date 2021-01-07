// Function to read binary files
#include <iostream>
#include <sstream> 
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <fstream>
#include <vector>
#include <iterator>
#include "../headers/common.hpp"
#include "../headers/handle-input.hpp"
using namespace std;
int** cluster_images;

void printFiles(uint32_t number_of_images, uint64_t d_original, uint64_t d) {
    cout << "INPUT ORIGINAL SPACE DATASET:" << endl;
    for (uint32_t i = 0; i < number_of_images; i++) {
        cout << "next image" << endl;
        for (uint64_t j = 0; j < d_original; j++) {
            cout << all_images_original_space[i][j] << '\t';
        }
        cout << endl;
    }
    
    cout << "INPUT NEW SPACE DATASET:" << endl;
    for (uint32_t i = 0; i < number_of_images; i++) {
        cout << "next image" << endl;
        for (uint64_t j = 0; j < d; j++) {
            cout << all_images_new_space[i][j] << '\t';
        }
        cout << endl;
    }
}

uint16_t *openMMap(string name, long &size) {
    int m_fd;
    struct stat statbuf;
    uint16_t *m_ptr_begin;

    if ((m_fd = open(name.c_str(), O_RDONLY)) < 0) {
        perror("can't open file for reading");
    }
    if (fstat(m_fd, &statbuf) < 0) {
        perror("fstat in openMMap failed");
    }
    if ((m_ptr_begin = (uint16_t *)mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, m_fd, 0)) == MAP_FAILED) {
        perror("mmap in openMMap failed");
    }
    uint16_t *m_ptr = m_ptr_begin;
    size = statbuf.st_size;
    return m_ptr;
}

// handling the input file
void readFile(const string& filename, int file_type, uint32_t* number_of_images, uint64_t* d) {
    long length;
    uint16_t *mmfile = openMMap(filename, length);
    uint32_t* memblockmm;
    uint16_t* buffer;
    memblockmm = (uint32_t *)mmfile; //cast file to uint32 array
    uint32_t magic_number = memblockmm[0]; // take first int
    mmfile += 2; //increment by 4 as I read a 32 bit entry and each entry in mmfile is 8 bits. The sizeof mmfile is 2 bytes. So 2*2=4, the offset we need.
    uint32_t image_number = memblockmm[1];
    mmfile += 2;
    uint32_t number_of_rows = memblockmm[2];
    mmfile += 2;
    uint32_t number_of_columns = memblockmm[3];
    mmfile += 2;

    magic_number = __builtin_bswap32(magic_number);
    image_number = __builtin_bswap32(image_number);
    number_of_rows = __builtin_bswap32(number_of_rows);
    number_of_columns = __builtin_bswap32(number_of_columns);
    
    *d = number_of_columns * number_of_rows;
    *number_of_images = image_number;
    buffer = static_cast<uint16_t *>(mmfile);

    int dimensions = *d;
    int offset = 0;
    all_images_new_space = new int *[image_number];
    for (int image = 0; image < (int)image_number; image++) {
        all_images_new_space[image] = new int[dimensions];
        for (int i = 0; i < dimensions; i++) {
            all_images_new_space[image][i] = (int)buffer[offset];
            offset++;
        }
    }

    munmap(mmfile, length);
}

uint8_t *openMMapOriginalSpace(string name, long &size) {
    int m_fd;
    struct stat statbuf;
    uint8_t *m_ptr_begin;

    if ((m_fd = open(name.c_str(), O_RDONLY)) < 0) {
        perror("can't open file for reading");
    }
    if (fstat(m_fd, &statbuf) < 0) {
        perror("fstat in openMMap failed");
    }
    if ((m_ptr_begin = (uint8_t *)mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, m_fd, 0)) == MAP_FAILED) {
        perror("mmap in openMMap failed");
    }
    uint8_t *m_ptr = m_ptr_begin;
    size = statbuf.st_size;
    return m_ptr;
}

void readFileOriginalSpace(const string& filename, int file_type, uint32_t* number_of_images, uint64_t* d) {
    long length;
    uint8_t *mmfile = openMMapOriginalSpace(filename, length);
    uint32_t* memblockmm;
    uint8_t* buffer;
    memblockmm = (uint32_t *)mmfile; //cast file to uint32 array
    uint32_t magic_number = memblockmm[0]; // take first int
    mmfile += 4; //increment by 4 as I read a 32 bit entry and each entry in mmfile is 8 bits.
    uint32_t image_number = memblockmm[1];
    mmfile += 4;
    uint32_t number_of_rows = memblockmm[2];
    mmfile += 4;
    uint32_t number_of_columns = memblockmm[3];
    mmfile += 4;

    magic_number = __builtin_bswap32(magic_number);
    image_number = __builtin_bswap32(image_number);
    number_of_rows = __builtin_bswap32(number_of_rows);
    number_of_columns = __builtin_bswap32(number_of_columns);
    
    *d = number_of_columns * number_of_rows;
    *number_of_images = image_number;
    // change memblockmm to int *
    buffer = static_cast<uint8_t *>(mmfile);

    int dimensions = *d;
    int offset = 0;
    all_images_original_space = new int *[image_number];
    for (int image = 0; image < (int)image_number; image++) {
        all_images_original_space[image] = new int[dimensions];
        for (int i = 0; i < dimensions; i++) {
            all_images_original_space[image][i] = (int)buffer[offset];
            offset++;
        }
    }

    munmap(mmfile, length);
}

void readConfFile(string filename, int* K_medians) {
    ifstream confFile(filename);
    string param;
    int value, k, L;
    while (confFile >> param >> value) {
        if(param == "number_of_clusters:") {
            *K_medians = value;
        }
        else if(param == "number_of_vector_hash_tables:") {
            L = value;
        }
        else if(param == "number_of_vector_hash_functions:") {
            k = value;
        }
    }
}

vector<pair<int*, vector<int> > > readClusterFile(string filename, int K) {
    vector<vector<int> > temp(K);
    vector<pair<int*, vector<int> > > clusters;
    int coords[10] = {};
    ifstream clusterFile(filename);
    string cluster_N, token, size_N, size_V, image_N;
    string newString;
    int _size, i, image_number;
    int counter = 1;
    while (clusterFile >> cluster_N >> token >> size_N >> size_V) {
        if (cluster_N.compare("CLUSTER-" + to_string(counter)) != 0 || token.compare("{") != 0 || size_N.compare("size:") != 0){
            cerr << "Something went wrong while reading the cluster file!" << endl;
            exit(ERROR);
        }
        newString = size_V.substr(0, size_V.size()-1);
        stringstream st (newString);
        st >> _size;
        if(_size < 0) {
            cerr << "size of cluster is less than 0!" << endl;
            exit(ERROR);
        }
        i = 0;
        while (clusterFile >> image_N) {
            /*
                In clusters we have firstly: the coords of the centroid, 
                secondly: the indexes of the neighbours
            */
            // cut the comma (last char of image_N), make it an int and push it to temp
            // store the image_N in the
            newString = image_N.substr(0, image_N.size()-1);
            stringstream st1 (newString);
            st1 >> image_number;
            // 
            temp[counter - 1].push_back(image_number);

            i++;
            if (i == _size){
                break;
            }
        }
        counter++;
    }
    if (counter != K+1){
        cerr << "K given is different than the number of classes read from cluster file!" << endl;
        exit(ERROR);
    }
    // store them in cluster
    // first will be an empty int * array, because we don't have centroid and its coords
    for (unsigned int i = 0; i < K; i++) {
        clusters.push_back(make_pair(coords, temp[i]));
    }
    return clusters;
}
