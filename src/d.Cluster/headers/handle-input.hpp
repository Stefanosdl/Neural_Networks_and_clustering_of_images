#include <stdint.h>
#include <fstream>

extern int** all_images_original_space;
extern int** all_images_new_space;

void handleInput(int , char**, uint32_t*, uint64_t*, int*, std::string*);
void readFile(const std::string&, int, uint32_t*, uint64_t*);
void readFileOriginalSpace(const std::string&, int, uint32_t*, uint64_t*);
void printFiles(uint32_t, uint64_t, uint64_t);
void readConfFile(std::string , int *);
uint16_t *openMMap(std::string, long &);
void readClusterFile(std::string, int);