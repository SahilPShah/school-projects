#include <vector>
#include "Hash.h"

void doubleHashInput(std::vector<int> &v, int elem){
    int idx = firstHash(elem, v.size());
    if(v[idx] = -1)
        v[idx] = elem;
    else{
        while(v[idx] != -1)
            int idx = idx + secondHash(elem) % v.size();
        v[idx] = elem;
    }
}

//make a hash function called firstHash
//make a second function called secondHash

int firstHash(int elem, int length){
    return length%(elem*4);
}

int secondHash(int elem){
    return 3-(elem%3);
}
