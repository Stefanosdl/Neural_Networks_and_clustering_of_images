#include <vector>
#include <stdint.h>
#include <fstream>

#define HASHTABLE_NUMBER 16
#define SMALL_K 4
#define SMALL_L 5
#define N 1

extern int **all_images;
extern int **query_images;
extern int **all_images_original_space;
extern int **query_images_original_space;

void handleInput(int , char**, uint32_t*, uint64_t*, int*, int*, std::string*, std::string*, std::string*);
void handleReExecution(uint32_t*, uint64_t*, int*, int*, std::string*, std::string*, std::string*);
void readFile(const std::string&, int, uint32_t*, uint64_t*, int, int);
void readFileOriginalSpace(const std::string&, int, uint32_t*, uint64_t*, int, int);
void printFiles(uint32_t, uint32_t, uint64_t);
uint16_t *openMMap(std::string, long &);