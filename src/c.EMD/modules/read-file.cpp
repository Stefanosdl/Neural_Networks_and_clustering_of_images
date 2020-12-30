// Function to read binary files
#include <iostream>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <fstream>
#include "../headers/common.hpp"
#include "../headers/handle-input.hpp"
using namespace std;

void printFiles(uint32_t number_of_images, uint32_t number_of_query_images, uint64_t d_original) {
    cout << "INPUT ORIGINAL SPACE DATASET:" << endl;
    for (uint32_t i = 0; i < number_of_images; i++) {
        cout << "next image" << endl;
        for (uint64_t j = 0; j < d_original; j++) {
            cout << all_images_original_space[i][j] << '\t';
        }
        cout << endl;
    }
    cout << "QUERY ORIGINAL SPACE DATASET:" << endl;
    for (uint32_t i = 0; i < number_of_query_images; i++) {
        cout << "next image" << endl;
        for (uint64_t j = 0; j < d_original; j++) {
            cout << query_images_original_space[i][j] << '\t';
        }
        cout << endl;
    }
    cout << "INPUT LABELS:" << endl;
    for (uint32_t i = 0; i < number_of_images; i++) {
        cout << all_images_labels[i] << endl;
    }
    cout << "QUERY LABELS:" << endl;
    for (uint32_t i = 0; i < number_of_query_images; i++) {
        cout << query_images_labels[i] << endl;
    }
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
    int dimensions = 0;
    uint8_t *mmfile = openMMapOriginalSpace(filename, length);
    uint32_t* memblockmm;
    uint8_t* buffer;
    memblockmm = (uint32_t *)mmfile; //cast file to uint32 array
    uint32_t magic_number = memblockmm[0]; // take first int
    mmfile += 4; //increment by 4 as I read a 32 bit entry and each entry in mmfile is 8 bits.
    uint32_t image_number = memblockmm[1];
    mmfile += 4;
    if (file_type != INPUT_LABELS && file_type != QUERY_LABELS) {
        uint32_t number_of_rows = memblockmm[2];
        mmfile += 4;
        uint32_t number_of_columns = memblockmm[3];
        mmfile += 4;
        number_of_rows = __builtin_bswap32(number_of_rows);
        number_of_columns = __builtin_bswap32(number_of_columns);
        *d = number_of_columns * number_of_rows;
        dimensions = *d;
    }

    magic_number = __builtin_bswap32(magic_number);
    image_number = __builtin_bswap32(image_number);
    
    *number_of_images = image_number;
    // change memblockmm to int *
    buffer = static_cast<uint8_t *>(mmfile);

    int offset = 0;
    if (file_type == INPUT_FILE) {
        all_images_original_space = new int *[image_number];
        for (int image = 0; image < (int)image_number; image++) {
            all_images_original_space[image] = new int[dimensions];
            for (int i = 0; i < dimensions; i++) {
                all_images_original_space[image][i] = (int)buffer[offset];
                offset++;
            }
        }
    }
    else if (file_type == QUERY_FILE) {
        // initialize the array for the query dataset
        query_images_original_space = new int *[image_number];
        // loop over all images to read them
        for (int image = 0; image < (int)image_number; image++) {
            query_images_original_space[image] = new int[dimensions];
            for (int i = 0; i < dimensions; i++) {
                query_images_original_space[image][i] = (int)buffer[offset];
                offset++;
            }
        }
    }
    else if (file_type == INPUT_LABELS) {
        all_images_labels = new int [image_number];
        for (int image = 0; image < (int)image_number; image++) {
            all_images_labels[image] = (int)buffer[offset];
            offset++;
        }
    }
    else if (file_type == QUERY_LABELS) {
        query_images_labels = new int [image_number];
        for (int image = 0; image < (int)image_number; image++) {
            query_images_labels[image] = (int)buffer[offset];
            offset++;
        }
    }
    
    munmap(mmfile, length);
}