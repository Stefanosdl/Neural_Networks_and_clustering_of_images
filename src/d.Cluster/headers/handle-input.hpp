#include <stdint.h>
#include <fstream>
#include <vector>

extern int** all_images_original_space;
extern int** all_images_new_space;

void handleInput(int , char**, uint32_t*, uint64_t*, uint64_t*, int*, std::string*, std::string*);
void readFile(const std::string&, uint32_t*, uint64_t*);
void readFileOriginalSpace(const std::string&, uint32_t*, uint64_t*);
void printFiles(uint32_t, uint64_t, uint64_t);
void readConfFile(std::string , int *);
uint16_t *openMMap(std::string, long &);
std::vector<std::pair<int *, std::vector<int>>> readClusterFile(std::string, int, uint64_t);