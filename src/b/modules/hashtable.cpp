#include <iostream>

#include "../headers/hashtable.hpp"
#include "../headers/modulo.hpp"
#include "../headers/common.hpp"
#include "../headers/handle-input.hpp"

// it is a vector of hashtables
// each hashtable is represented as a vector of vectors
// and its values in each bucket is a pair or <image, g_x> (int, unsigned int)
vector<vector<vector<pair<int, unsigned int> > > > HashTables;

void initializeHashtables(int L, uint32_t n) {
	int hashtable_size = n/HASHTABLE_NUMBER;
    HashTables.resize(L, vector<vector<pair<int, unsigned int> > >(hashtable_size, vector<pair<int, unsigned int> >(0)));
}

void insertToHashtable(int L, uint32_t image, unsigned int g_x, uint32_t n) {
	int hashtable_size = n/HASHTABLE_NUMBER;
	unsigned long pos = customModulo(g_x, hashtable_size);
	HashTables[L][pos].push_back(make_pair(static_cast<unsigned int>(image), g_x));
}

void printHashtable(int L, int n) {
	int hashtable_size = n/HASHTABLE_NUMBER;
	for(int i=0; i<L; i++) {
		cout<<"Current Hashtable: "<<i<<endl;
		for(int j=0; j<hashtable_size; j++) {
			cout<<"Current Bucket: "<<j<<endl;
			for(unsigned int k=0; k<HashTables[i][j].size(); k++) {
				cout<<HashTables[i][j][k].first << " g_x=" << HashTables[i][j][k].second <<'\t';
			}
			cout << endl;
		}
	}
}

void deleteHashtable(int L, int n) {
    int hashtable_size = n/HASHTABLE_NUMBER;
    for (int i = 0; i < L;i++) {
        for (int j = 0; j < hashtable_size;j++) {
            HashTables[i][j].clear();
        }
        HashTables[i].clear();
    }
    HashTables.clear();
}