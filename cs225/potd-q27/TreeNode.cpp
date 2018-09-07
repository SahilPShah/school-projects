#include "TreeNode.h"

#include <cstddef>
#include <iostream>
using namespace std;

TreeNode::TreeNode() : left_(NULL), right_(NULL) { }

int TreeNode::getHeight(){
  if(!this){
    return -1;
  }
  return 1+max(this->right_->getHeight(), this->left_->getHeight());
}
