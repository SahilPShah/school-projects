#include <vector>
#include "BTreeNode.h"


std::vector<int> traverse(BTreeNode* root) {
    // your code here
    std::vector<int> v;
    if(root)
      traverse(root, v);
    return v;
}

void traverse(BTreeNode* root, std::vector<int>& v){
  for(unsigned i =0; i<=root->elements_.size();i++){
    if(!root->is_leaf_)
      traverse(root->children_[i], v);
    if(i<root->elements_.size())
      v.push_back(root->elements_[i]);
  }
}
