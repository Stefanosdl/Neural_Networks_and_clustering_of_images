#include <stdint.h>
#include <vector>
#include <unordered_map>
#include <fstream>

void Brute_Force(uint64_t , uint32_t , int , std::unordered_map<int, std::vector<int> > &);
void Evaluate_Results(int, std::unordered_map<int, std::vector<int> > &, std::ofstream *);