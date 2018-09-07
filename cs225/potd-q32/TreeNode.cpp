#include "TreeNode.h"
#include <algorithm>

using namespace std;
TreeNode* findLastUnbalanced(TreeNode* root) {
 TreeNode* point = NULL;
 int deepest = -1;
 int level = 0;
 find(root, level, deepest, point);
 return point;

}

void deleteTree(TreeNode* root)
{
  if (root == NULL) return;
  deleteTree(root->left_);
  deleteTree(root->right_);
  delete root;
  root = NULL;
}

int getHeight(TreeNode* root){
  if(!root){
    return -1;
  }
  return 1+max(getHeight(root->right_), getHeight(root->left_));
}

int getDepth(TreeNode* root, TreeNode* subroot){
  if(root->val_ == subroot->val_)
    return 1;
  else if(root->val_ < subroot->val_)
    return 1+getDepth(root->right_, subroot);
  else
    return 1+getDepth(root->left_,subroot);
}


bool isBalanced(TreeNode* subroot){
  return abs(getHeight(subroot->right_) - getHeight(subroot->left_)) <=1;
}
void find(TreeNode* subroot, int level, int deepest, TreeNode*& point){
  if(subroot){
    find(subroot->left_, level, deepest, point);
    find(subroot->right_, level, deepest, point);
    if(level > deepest && !isBalanced(subroot)){
      point = subroot;
      deepest = level;
    }
  }
}
