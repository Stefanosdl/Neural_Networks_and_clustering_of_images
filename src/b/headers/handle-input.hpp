#include <vector>
#include <stdint.h>
#include <fstream>

#define HASHTABLE_NUMBER 16
#define SMALL_K 4
#define SMALL_L 5
#define N 1

// giving universal access to the all_images array
extern int **all_images;
extern int **query_images;

void handleInput(int , char**, uint32_t*, uint64_t*, int*, int*, std::string*, std::string*, std::string*);
void handleReExecution(uint32_t*, uint64_t*, int*, int*, std::string*, std::string*, std::string*);
void readFile(const std::string&, int, uint32_t*, uint64_t*, int, int);
void printFiles(uint32_t, uint32_t, uint64_t);
uint8_t *openMMap(std::string, long &);