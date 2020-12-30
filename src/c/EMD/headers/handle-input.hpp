#include <stdint.h>
#include <fstream>

#define N 10

extern int **all_images_original_space;
extern int **query_images_original_space;
extern int **all_images_images_labels;
extern int **query_images_labels;

void handleInput(int , char**, uint32_t*, uint64_t*, std::string*, std::string*, std::string*);
void handleReExecution(uint32_t*, uint64_t*, std::string*, std::string*, std::string*);
void readFileOriginalSpace(const std::string&, int, uint32_t*, uint64_t*);
void printFiles(uint32_t, uint32_t, uint64_t, uint64_t);
uint16_t *openMMap(std::string, long &);