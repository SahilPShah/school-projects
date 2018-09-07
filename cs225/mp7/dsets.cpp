#include "dsets.h"

void DisjointSets::addelements(int num) {
    for(int i = 0; i < num; i++)
        sets_.push_back(-1);    // adds unconnected node to the sets vector num times
}

int DisjointSets::find(int elem) {
    if(sets_[elem] < 0)  // if there is a negative index
        return elem;
    else {
        sets_[elem] = find(sets_[elem]);    //index the value held in the original parameter
        return sets_[elem];
    }
}

void DisjointSets::setunion(int a, int b) {
    int rootA = find(a);        // find the root of the up-tree with a in it
    int rootB = find(b);        // find the root of the up-tree with b in it
    if(rootA == rootB)          // if a and b are already in the same up-tree
        return;
    int newSize = sets_[rootA] + sets_[rootB];  // this is the negated new size
    if(sets_[rootA] <= sets_[rootB]) {            // union by size
        sets_[rootB] = rootA;
        sets_[rootA] = newSize;
    }
    else {
        sets_[rootA] = rootB;
        sets_[rootB] = newSize;
    }

}

int DisjointSets::size(int elem) {
    int size = sets_[find(elem)];
    return -1 * size;
}

void DisjointSets::clear() {
    if(!sets_.empty())
      sets_.clear();
}
