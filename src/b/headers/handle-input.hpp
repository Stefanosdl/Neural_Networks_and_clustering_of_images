#include <vector>
#include <stdint.h>
#include <fstream>

#define HASHTABLE_NUMBER 16
#define SMALL_K 4
#define SMALL_L 5

// giving universal access to the all_images array
extern unsigned char **all_images;
extern unsigned char **query_images;

void handleInput(int , char**, uint32_t*, uint64_t*, int*, int*, int*, unsigned int*, std::string*, std::string*);
void handleReExecution(uint32_t*, uint64_t*, int*, int*, int*, unsigned int*, std::string*, std::string*);
void readFile(const std::string&, int, uint32_t*, uint64_t*, int, int);
void printFiles(uint32_t, uint32_t, uint64_t);
uint8_t *openMMap(std::string, long &);