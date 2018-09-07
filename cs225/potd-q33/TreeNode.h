#ifndef TreeNode_H
#define TreeNode_H

#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val_;
    TreeNode *left_;
    TreeNode *right_;
    TreeNode *parent_;
    TreeNode(int x) {
      left_ = NULL;
      right_ = NULL;
      val_ = x;
      parent_ = NULL;
    }
};

void rightRotate(TreeNode*& t);
void leftRotate(TreeNode*& t);
//returns the height
int getHeight(TreeNode* subroot);


//true if node is balanced, false if not balanced
bool isBalanced(TreeNode* subroot);

int getDepth(TreeNode* root);

void deleteTree(TreeNode* root);

void find(TreeNode* subroot, int level, int deepest, TreeNode*& point);

TreeNode* findLastUnbalanced(TreeNode* root);

int getBalance(TreeNode* root);

#endif
