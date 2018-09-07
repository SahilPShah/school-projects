#include "TreeNode.h"
#include <algorithm>
using namespace std;
int getHeightBalance(TreeNode* root) {
  // your code here
  return root->right_->getHeight() - root->left_->getHeight();
  return 0;
}

void deleteTree(TreeNode* root)
{
  if (root == NULL) return;
  deleteTree(root->left_);
  deleteTree(root->right_);
  delete root;
  root = NULL;
}

int TreeNode::getHeight(){
    if(!this){
      return -1;
    }
    return 1+max(this->right_->getHeight(), this->left_->getHeight());
}
