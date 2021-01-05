#include <stdint.h>
#include <fstream>

#define HASHTABLE_NUMBER 16
#define SMALL_K 4
#define SMALL_L 3
#define N 1

extern int** all_images_original_space;
extern int** all_images_new_space;

void handleInput(int , char**, uint32_t*, uint64_t*, int*, int*, int*, std::string*);
void handleReExecution(uint32_t*, uint64_t*, int*, int*, int*, std::string*);
void readFile(const std::string&, int, uint32_t*, uint64_t*, int, int);
void readFileOriginalSpace(const std::string&, int, uint32_t*, uint64_t*, int, int);
void printFiles(uint32_t, uint64_t, uint64_t);
void readConfFile(std::string , int *, int *, int *);
uint16_t *openMMap(std::string, long &);
