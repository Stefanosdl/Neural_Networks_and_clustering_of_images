// uint32_t undefined reference.
#include <stdint.h>
#include <vector>

void initializeHashtables(int, uint32_t);
void insertToHashtable(int, uint32_t, unsigned int, uint32_t);
void printHashtable(int, int);
void deleteHashtable(int, int);

using namespace std;
extern vector<vector<vector<pair<int, unsigned int> > > > HashTables;
