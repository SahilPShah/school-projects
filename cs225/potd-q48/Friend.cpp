#include "Friend.h"
#include <iostream>

int find(int x, std::vector<int>& parents) {
   if(parents[x] < 0)
     return x;
   else
     return find(parents[x], parents);
}

int findCircleNum(std::vector<std::vector<int>>& M) {
    std::vector<int> parents;
    for(unsigned i = 0; i < M.size(); i++)
      parents.push_back(-1);
    for(unsigned i = 0; i < M.size(); i++) {
      for(unsigned j = i + 1; j < M.size(); j++) {
        if(M[i][j] && parents[j] == -1)
          parents[j] = i;
        else if(M[i][j] && parents[j] != -1) {
          int idx = find(j, parents);
          int idx2 = find(parents[j], parents);
          parents[idx] = idx2;
        }
      }
    }
    int num_groups = 0;
    for(unsigned i = 0; i < parents.size(); i++) {
      if(parents[i] == -1)
        num_groups++;
    }
    return num_groups;
}
