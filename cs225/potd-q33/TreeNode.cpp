#include "TreeNode.h"
#include <algorithm>
#include <cmath>
using namespace std;
//just rotate here, imbalacnce and rotation will be determined in main
void rightRotate(TreeNode*& t) {
  TreeNode* top = t->left_;
  top->parent_ = t->parent_;
  TreeNode* c = top->right_;
  top->right_ = t;
  t->parent_ = top;
  t->left_ = c;
  c->parent_ = t;
  t = top;
}


void leftRotate(TreeNode*& t) {
  TreeNode* top = t->right_;
  top->parent_ = t->parent_;
  TreeNode* b = top->left_;
  top->left_ = t;
  t->parent_ = top;
  t->right_ = b;
  b->parent_ = t;
  t = top;

}



void deleteTree(TreeNode* root)
{
  if (root == NULL) return;
  deleteTree(root->left_);
  deleteTree(root->right_);
  delete root;
  root = NULL;
}

int getHeight(TreeNode* root) {
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

TreeNode* findLastUnbalanced(TreeNode* root) {
 TreeNode* point = NULL;
 int deepest = -1;
 int level = 0;
 find(root, level, deepest, point);
 return point;

}

int getBalance(TreeNode* root){
  return getHeight(root->right_) - getHeight(root->left_);
}
