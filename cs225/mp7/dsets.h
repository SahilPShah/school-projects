#ifndef DSETS_H_
#define DSETS_H_

#include <vector>
using namespace std;

class DisjointSets {
public:
    /*
    * creates num unconnected root nodes at the end of the vector sets_
    */
    void addelements(int num);

    /*
    * finds the index of the root of the up-tree that elem is a part of and performs path compression
    */
    int find(int elem);

    /*
    * unions the up-trees containing elements a and b
    * performs a union by size (when the sizes of the up-trees are the same,
    * the second up-tree points to the first)
    */
    void setunion(int a, int b);

    /*
    * returns the size of the up-tree where elem resides (finds the
    * root index of the up-tree and returns the negative of the integer stored there)
    */
    int size(int elem);

    /*
    * clears the sets vector if it is not already empty;
    */
    void clear();

private:
    vector<int> sets_;  // the array that represents the up-trees of the disjoint set
};

#endif
