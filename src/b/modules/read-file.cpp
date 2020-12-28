// Function to read binary files
#include <iostream>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <fstream>
#include <vector>
#include <iterator>
#include "../headers/common.hpp"
#include "../headers/handle-input.hpp"
#include "../headers/hashtable.hpp"
#include "../headers/manhattan-hashing.hpp"
using namespace std;

void printFiles(uint32_t number_of_images, uint32_t number_of_query_images, uint64_t d_original, uint64_t d) {
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
    cout << "INPUT NEW SPACE DATASET:" << endl;
    for (uint32_t i = 0; i < number_of_images; i++) {
        cout << "next image" << endl;
        for (uint64_t j = 0; j < d; j++) {
            cout << all_images[i][j] << '\t';
        }
        cout << endl;
    }
    cout << "QUERY NEW SPACE DATASET:" << endl;
    for (uint32_t i = 0; i < number_of_query_images; i++) {
        cout << "next image" << endl;
        for (uint64_t j = 0; j < d; j++) {
            cout << query_images[i][j] << '\t';
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
void readFile(const string& filename, int file_type, uint32_t* number_of_images, uint64_t* d, int k, int L) {
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
    
    cout << magic_number << '\n';
    cout << image_number << '\n';
    cout << number_of_rows << '\n';
    cout << number_of_columns << '\n';

    int dimensions = *d;
    int offset = 0;
    if (file_type == INPUT_FILE) {
        initializeHashtables(L, image_number);
        unsigned int g_x = 0;
        all_images = new int *[image_number];
        for (int image = 0; image < (int)image_number; image++) {
            all_images[image] = new int[dimensions];
            for (int i = 0; i < dimensions; i++) {
                all_images[image][i] = (int)buffer[offset];
                offset++;
                if(image < 1){
                    cout <<all_images[image][i] << '\t';
                }
            }
            if(image < 1){
                cout << endl;
            }
            for (int l = 0; l < L; l++) {
                g_x = calculateG_X(k, dimensions, image, INPUT_FILE);
                // pass to hashtable
                insertToHashtable(l, image, g_x, image_number);
            }
        }
    }
    else if (file_type == QUERY_FILE) {
        // initialize the array for the query dataset
        query_images = new int *[image_number];
        // loop over all images to read them
        for (int image = 0; image < (int)image_number; image++) {
            query_images[image] = new int[dimensions];
            for (int i = 0; i < dimensions; i++) {
                query_images[image][i] = (int)buffer[offset];
                offset++;
                if(image < 1){
                    cout <<query_images[image][i] << '\t';
                }
            }
            if(image < 1){
                cout << endl;
            }
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

void readFileOriginalSpace(const string& filename, int file_type, uint32_t* number_of_images, uint64_t* d, int k, int L) {
    long length;
    uint8_t *mmfile = openMMapOriginalSpace(filename, length);
    cout<<"ORIGINAL INPUT"<<endl;
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
    if (file_type == INPUT_FILE) {
        initializeHashtables(L, image_number);
        unsigned int g_x = 0;
        all_images_original_space = new int *[image_number];
        for (int image = 0; image < (int)image_number; image++) {
            all_images_original_space[image] = new int[dimensions];
            for (int i = 0; i < dimensions; i++) {
                all_images_original_space[image][i] = (int)buffer[offset];
                offset++;
                if(image < 1){
                    cout <<all_images_original_space[image][i] << '\t';
                }
            }
            if(image < 1){
                cout << endl;
            }

            for (int l = 0; l < L; l++) {
                g_x = calculateG_X(k, *d, image, INPUT_FILE);
                // pass to hashtable
                insertToHashtable(l, image, g_x, image_number);
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
                if(image < 1){
                    cout <<query_images_original_space[image][i] << '\t';
                }
            }
            if(image < 1){
                cout << endl;
            }
        }

    }

    munmap(mmfile, length);
}