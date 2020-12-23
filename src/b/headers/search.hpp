#include <stdint.h>
#include <fstream>
#include <vector>

using namespace std;

void approximateN_NNs(ofstream*, uint64_t, int, int, int, uint32_t, int);
vector<pair<unsigned int, unsigned int> > approximateN_NNs_Full_Search(uint64_t, int, uint32_t, int);
void rangeSearch(ofstream*, uint64_t, int, int, uint32_t, unsigned int, int);
