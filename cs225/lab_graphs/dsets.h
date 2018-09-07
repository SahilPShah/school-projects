/* Your code here! */
#ifndef DSETS_H_
#define DSETS_H_
#include <vector>
using namespace std;
class DisjointSets{
public:
    /*
    *creates num unconnected root nodes at the end of the vector
    */
    void addelements(int num);

    /*
    * fi ds the index of the root of the up-tree where elem is a part of
    */
    int find(int elem);

    /*
    *unions the up-trees rooted at a and b
    *performs a union by size
    */
    void setunion(int a, int b);

    /*
    *the size of the up-tree where elem resides
    */
    int size(int elem);
private:
    vector<int> sets_;
};
#endif
