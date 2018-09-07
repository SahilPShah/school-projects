// your code here!
#include <cmath>
#include "potd.h"
using namespace potd;

int *potd::raise(int *arr){
    int size = 1;
    while(arr[size]>=0){
        size++;
    }
    int *output = new int[size+1];
    for(int i=0;i<size+1;i++){
        output[i]=arr[i];
    }
    for(int i=0;i<size-1;i++){
        output[i] = pow(output[i],output[i+1]);
    }
    return output;
}
