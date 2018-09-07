#ifndef TreeNode_H
#define TreeNode_H

#include <cstddef>

// Definition for a binary tree node.
struct TreeNode {
    int val_;
    int balance_;
    TreeNode *left_;
    TreeNode *right_;
    TreeNode(int x) {
      left_ = NULL;
      right_ = NULL;
      val_ = x;
      balance_ = 0;
    }
};

TreeNode* findLastUnbalanced(TreeNode* root);

//returns the height
int getHeight(TreeNode* subroot);


//true if node is balanced, false if not balanced
bool isBalanced(TreeNode* subroot);

int getDepth(TreeNode* root);

void deleteTree(TreeNode* root);

void find(TreeNode* subroot, int level, int deepest, TreeNode*& point);

#endif
